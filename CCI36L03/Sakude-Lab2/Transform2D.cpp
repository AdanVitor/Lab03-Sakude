
TO DO move class definion to header
class MATRIX2D
{
  float mat[3][2];
  float Ang,Sx,Sy,Cos_a,Sin_a,Tx,Ty;
public:
  MATRIX2D();
  void Identity();
  void SetValue(int i,int j, float value); 
  void Translation(float tx, float ty);
  void Scale(float sx,float sy);
  void Rotation(float ang);
  void Rotation(float cos_a, float sin_a);
  void DoTransformation( float &x, float &y);
  void BuildTransformation(float sx,float sy,float ang, float tx,float ty);
  
};


  
MATRIX2D::MATRIX2D()
{
	Identity();
	Ang=0.0f;
	Sx=Sy=1.0f;
	Cos_a=1.0f; Sin_a=0.0f;
	Tx=Tx=0.0f;

}
void MATRIX2D::Identity()
{  
  	mat[0][0]=1.0f; mat[0][1]=0.0f;
	mat[1][0]=0.0f; mat[1][1]=1.0f;
	mat[2][0]=0.0f; mat[2][0]=0.0f;
}
void MATRIX2D::SetValue(int i,int j, float value)
{
  if ((i>=0 && i<3)&&(j>=0&&(j<2)))
	  mat[i][j]=value;
}

void MATRIX2D::Translation(float tx, float ty)
{   Tx=tx;
    Ty=ty;
    mat[2][0]+=tx;
    mat[2][1]+=ty;
}
void MATRIX2D::Scale(float sx,float sy)
{
	Sx=sx;
	Sy=sy;
	for(int i=0; i<3; i++)
	{mat[i][0]*=sx;
	mat[i][1]*=sy;
	}
}
void MATRIX2D::Rotation(float ang)
{
  Rotation((float) cos(ang), (float) sin(ang));
}
void MATRIX2D::Rotation(float cos_a, float sin_a)
{ float temp;
   Cos_a=cos_a;
   Sin_a=sin_a;
   for(int i=0; i<3; i++)
   {
	  temp=mat[i][0]*Cos_a-mat[i][1]*Sin_a;
	  mat[i][1]=mat[i][0]*Sin_a+mat[i][1]*Cos_a;
	  mat[i][0]=temp;
   }
}
void MATRIX2D::DoTransformation( float &x, float &y)
{ float temp;
  temp=x*mat[0][0]+y*mat[1][0]+mat[2][0];
  y=x*mat[0][1]+y*mat[1][1]+mat[2][1];
  x=temp;
}
void MATRIX2D::BuildTransformation(float sx,float sy,
  float ang, float tx,float ty)
{
   Identity();
   Scale(sx,sy);
   Rotation(ang);
   Translation(tx,ty);
}