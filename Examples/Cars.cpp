// cars.cpp - implementation of simple types of cars
// ============ TODO2: Complete this file to make it look natural ===============



#include "../GrTown_PCH.H"
#include "../DrawUtils.H"
#include "Utilities/Texture.H"
#include "../DrawingState.H"
#include "Cars.H"

/////////////////////////////////////////////////////
// car colors have names too
struct CarColors {
	char* name;
	int r,g,b;
};
CarColors cc[] = {
	{"red",220,30,30},
	{"white",225,225,225},
	{"black",50,50,70},
	{"green",30,225,30},
	{"blue",30,30,225},
	{"yellow",225,225,30}
};
const int nCarCols = 6;

///////////////////////////////////////////////////////
// the generic car...
int carCtr = 0;
Car::Car(char* name, int c) : GrObject(name,carCtr,cc[c%nCarCols].name)
{
  color((float) cc[c%nCarCols].r,(float) cc[c%nCarCols].g,(float) cc[c%nCarCols].b);
  rideAbove = 10;
  ridable = 1;

  // car params
	 w = 3.5;		// half of width
	 f = 3;		// length of front part
	 r = 6;		// length of back part
	 h = 1;		// height of ground
	 m = 1.5;		// height of motor
	 t = 4;		// height of back part
	 s = .3f;		// slope of windshield
	 a = .2f;		// rake of hood
	 sr = .1f;
	 br = .2f;

	 // note: we can't actually load the textures now (since we don't have a GL context)
	 // so we just say that we don't have textures
	frontTexture = 0;
	sideTexture = 0;
}

// draw a wheel
//square root of 2 over 2
const float sqrt2_2 = .707106f;
float wc[9][2] = {
	{0,1}, {sqrt2_2,sqrt2_2}, {1,0}, {sqrt2_2,-sqrt2_2},
	{0,-1}, {-sqrt2_2,-sqrt2_2}, {-1,0}, {-sqrt2_2,sqrt2_2},
	{0,1}
};
static void drawWheel(float r, float w)
{
	int i=0;
	glNormal3f(1,0,0);
	glBegin(GL_POLYGON);
	for(i=7; i>=0; i--)
		glVertex3f(w,wc[i][0]*r,wc[i][1]*r);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	for(i=0; i<9; i++) {
		glVertex3f(0,wc[i][0]*r,wc[i][1]*r);
		glVertex3f(w,wc[i][0]*r,wc[i][1]*r);
	}
	glEnd();

}

void Car::draw(DrawingState* d)
{

  // Draw four wheel 
  glColor3f(.1f,.1f,.1f);
  glPushMatrix();
	  glTranslatef(w-.5f,h,f-h);
	  drawWheel(h,1);
  glPopMatrix();
  glPushMatrix();
	  glTranslatef(w-.5f,h,f+r-h-1);
	  drawWheel(h,1);
  glPopMatrix();

  glPushMatrix();
	  glTranslatef(-w+.5f,h,f-h);
	  glRotatef(180,0,1,0);
	  drawWheel(h,1);
  glPopMatrix();
  glPushMatrix();
	  glTranslatef(-w+.5f,h,f+r-h-1);
	  glRotatef(180,0,1,0);
	  drawWheel(h,1);
  glPopMatrix();

  // Draw the body of the car
  glColor3fv(&color.r);
  drawBody(d);
}

  
static void drawBeam()
{
	const float cone_length = 20;
	int c;
	const float radius = 3;
	glBegin(GL_TRIANGLE_FAN);
	glColor4d(1,1,0,.8);
	glVertex3d(0,0,0);
    glColor4d(1,1,0,.2);
	for ( c=8; c>=0; c--) {
	  glNormal3f( wc[c][0], 0, wc[c][1]);
	  glVertex3f( wc[c][0]*radius, -cone_length, wc[c][1]*radius );
	}  		
	glEnd();
	glNormal3f(0,-1,0);
	glBegin(GL_POLYGON);
	for ( c=0; c<8; c++) {
	  glVertex3f( wc[c][0]*radius, -cone_length, wc[c][1]*radius );
	}  		
	glEnd();
}
// draw headlights at night
void Car::drawAfter(DrawingState* s)
{
  int tod = s->timeOfDay;
  bool daytime;
  if (tod >=6 && tod <= 19)
	daytime = true;
  else 
	daytime = false;
	
  if (!daytime) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);

	// Beam One
	glPushMatrix();			
	glTranslatef(-w*.7f,h+m/2,2);
	glRotated(90,1,0,0);
	drawBeam();
	glPopMatrix();
		// Beam 2
	glPushMatrix();			
	glTranslatef(w*.7f,h+m/2,2);
	glRotated(90,1,0,0);
	drawBeam();
	glPopMatrix();
	glFrontFace(GL_CW);
	glClearColor(1,1,1,1);
	glDisable(GL_BLEND);
  }
}

void Car::drawBody(DrawingState*)
{

	polygon(-4, -w,h,0., -w,h+t,0., w,h+t,0., w,h,0.);
	polygon(4, -w,h,r+f, -w,h+t,r+f, w,h+t,r+f, w,h,r+f);
	polygon(-4, w,t+h,0., -w,t+h,0., -w,t+h,f+r, w,t+h,f+r);
	polygon(4, w,h,0., -w,h,0., -w,h,f+r, w,h,f+r);
    polygon(-4, w,t+h,0., w,t+h,f+r, w,h,f+r, w,h,0.);
	polygon(4, -w,t+h,0., -w,t+h,f+r, -w,h,f+r, -w,h,0.);

	//=============================================================
	// Now the cars are simply boxes with different hights. 
    // TODO2: To make the cars look more realistic and add texutres. 

}


/***********************************************************************/
// simple car: a sport-utility
SUV::SUV(int c) : Car("SUV",c)
{
	 w = 3.5;		// half of width
	 f = 3;		// length of front part
	 r = 7;		// length of back part
	 h = 1;		// height of ground
	 m = 1.5;		// height of motor
	 t = 4;		// height of back part
	 s = .3f;		// slope of windshield
	 a = .2f;		// rake of hood
	 sr = .5f;
	 br = .5f;
}

// hatchback is just different parameters
HatchBack::HatchBack(int c) : Car("HatchBack",c)
{
	 w = 2.5;		// half of width
	 f = 2.5;		// length of front part
	 r = 5;		// length of back part
	 h = 1;		// height of ground
	 m = 1.5;		// height of motor
	 t = 3;		// height of back part
	 s = .4f;		// slope of windshield
	 a = .6f;		// rake of hood
	 sr = 1;
	 br = 2.5f;
}

// another set makes a van...
Van::Van(int c) : Car("Van",c)
{
	 w = 3;		// half of width
	 f = 2;		// length of front part
	 r = 8;		// length of back part
	 h = 1;		// height of ground
	 m = 2.5;	// height of motor
	 t = 5;		// height of back part
	 s = .3f;		// slope of windshield
	 a = 1.f;		// rake of hood
	 sr = .5f;
	 br = .5f;
}


