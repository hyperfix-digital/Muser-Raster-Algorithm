//
// Rasterer
//
// Anthony Mesa
//
  
#include<iostream>  
#include<vector>
#include<cmath>
#include<algorithm>

#define WIDTH 20
#define BUFF_SIZE (WIDTH * WIDTH)

int buffer[BUFF_SIZE];

void InitBuffer()
{
  for(int i = 0; i < WIDTH; i++)
  {
    for(int j = 0; j < WIDTH; j++)
    {
      buffer[(i * WIDTH) + j] = 0;
    }
  }
}

void PrintBuffer()
{
  for(int k = 0; k < WIDTH; k++)
  {
    for(int l = 0; l < WIDTH; l++)
    {
      std::cout << buffer[(k * WIDTH) + l] << " ";
    }
    std::cout << std::endl;
  }
}

void WriteFaceToBuffer(std::vector<int> face)
{
  for(int i = 0; i < 3; i++)
  {
    buffer[((face[(i * 3) + 1]) * WIDTH) + face[(i * 3)]] = face[(i * 3) + 2];
  }
}

std::vector<int> OrderFace(std::vector<int> face_data)
{
   if (face_data[1] > face_data[4]){
    std::swap(face_data[0], face_data[3]);
    std::swap(face_data[1],face_data[4]);
    std::swap(face_data[2],face_data[5]);
  }

  if (face_data[4] > face_data[7]){
    std::swap(face_data[3],face_data[6]);
    std::swap(face_data[4],face_data[7]);
    std::swap(face_data[5],face_data[8]);
  }
 
  if (face_data[1] > face_data[4]){
    std::swap(face_data[0],face_data[3]);
    std::swap(face_data[1],face_data[4]);
    std::swap(face_data[2],face_data[5]);
  }	
  
  return face_data;
}

class Point
{
  public:
  float x;
  float y;
  float z;
  
  Point(float _x, float _y, float _z)
  {
    x = _x;
    y = _y;
    z = _z;
  }
  
  void ToString()
  {
    std::cout << x << " " << y << " " << z << std::endl;	
  }
};

float Distance(Point *u, Point *v)
{
  return std::sqrt(std::pow((u->x - v->x), 2) + std::pow((u->y - v->y), 2));
}
//A, H, L
float Interpolate(Point *u, Point *v, Point *w)
{
    std::cout << "dha " << Distance(u, v) << std::endl;
    std::cout << "dhl " << Distance(w, v) << std::endl;
  return v->z + ((w->z - v->z) * (Distance(u, v) / Distance(w, v)));
}

float Slope(Point *u, Point *v)
{
  return (u->y - v->y) / (u->x - v->x);
}

float YIntercept(float m, Point *u)
{
  return u->y - (m * u->x);
}

float X(int y, float b, float slope)
{
  return (y - b) / slope;
}

void RasterizeBuffer(std::vector<int> face_data)
{
  for(int i=0; i < (int)face_data.size(); i++)
    std::cout << face_data.at(i) << ' ';
   
  face_data = OrderFace(face_data);
  
  std::cout << std::endl;
  std::cout << std::endl;
  
  for(int i=0; i < (int)face_data.size(); i++)
    std::cout << face_data.at(i) << ' ';

  std::cout << std::endl;
  std::cout << std::endl;

  Point *L = new Point(
    face_data[0],
    face_data[1],
    face_data[2]
  );	
  
  Point *M = new Point(
    face_data[3],
    face_data[4],
    face_data[5]
  );
  
  Point *H = new Point(
    face_data[6],
    face_data[7],
    face_data[8]
  );

    std::cout << "L = ";
    L->ToString();
    std::cout << "M = ";
    M->ToString();
    std::cout << "H = ";
    H->ToString();
    
    int omega;
    
    float m_a = Slope(H, L);
    float b_a = YIntercept(m_a, H);

    std::cout << "tet " << m_a << " " << b_a << std::endl;

    float m_b = Slope(H, M);
    float b_b = YIntercept(m_b, H);
    
    Point *A = new Point(0, 0, 0);
    Point *B = new Point(0, 0, 0); 

    for(omega = M->y; omega < (H->y ); omega++)
    {
      std::cout << "omega = " << omega << std::endl;

      A->y = omega;
      A->x = !std::isinf(m_a) ? X(omega, b_a, m_a) : H->x;

      std::cout << "a-x " << A->x << std::endl;
      A->z = Interpolate(A, H, L);
        std::cout << "a-z " << A->z << std::endl;

      B->y = omega;
      B->x = !std::isinf(m_b) ? X(omega, b_b, m_b) : M->x;
      B->z = Interpolate(B, H, M);

      std::cout << "A = ";
      A->ToString();
      std::cout << "B = ";
      B->ToString();

      if(A->x < B->x){
        for(int i = A->x; i < (B->x ); i++)
        {
            Point *T = new Point(i, omega, 0);
            T->z = Interpolate(T, A, B);
            std::cout << "T = ";
            T->ToString();
            buffer[(omega * WIDTH) + i] = T->z;
        }
      }
      else{
        for(int i = B->x; i < (A->x ); i++)
        {
            Point *T = new Point(i, omega, 0);
            T->z = Interpolate(T, A, B);
            std::cout << "T = ";
            T->ToString();
            buffer[(omega * WIDTH) + i] = T->z;
        }
      }

    }

    m_b = Slope(L, M);
    b_b = YIntercept(m_b, L);

    for(omega = M->y; omega > (L->y ); omega--)
    {
      std::cout << "omega = " << omega << std::endl;

      A->y = omega;
      A->x = !std::isinf(m_a) ? X(omega, b_a, m_a) : L->x;

      std::cout << "a-x " << A->x << std::endl;
      A->z = Interpolate(A, H, L);
        std::cout << "a-z " << A->z << std::endl;

      B->y = omega;
      B->x = !std::isinf(m_b) ? X(omega, b_b, m_b) : M->x;
      B->z = Interpolate(B, L, M);


      std::cout << "A = ";
      A->ToString();
      std::cout << "B = ";
      B->ToString();

      if(A->x > B->x){ std::swap(A, B);}
        for(int i = A->x; i < (B->x ); i++)
        {
            Point *T = new Point(i, omega, 0);
            T->z = Interpolate(T, A, B);
            std::cout << "T = ";
            T->ToString();
            buffer[(omega * WIDTH) + i] = T->z;
        }
    }

    delete(L);
    delete(M);
    delete(H);
    delete(A);
    delete(B);
}

int main()
{
  std::vector<int> face1 = { 0, 0, 9, 5, 15, 1, 0, 20, 6 };
  std::vector<int> face2 = { 0, 0, 9, 5, 15, 1, 14, 3, 5 };
  std::vector<int> face3 = { 0, 0, 9, 14, 3, 5, 20, 0, 3};  std::vector<int> face4 = { 5, 15, 1, 14, 3, 5, 20, 20, 2};
    std::vector<int> face5 = { 20, 0, 3, 14, 3, 5, 20, 20, 2};
    std::vector<int> face6 = { 0, 20, 6, 5, 15, 1, 20, 20, 2};
  
  InitBuffer();

  RasterizeBuffer(face1);
  RasterizeBuffer(face2);
  RasterizeBuffer(face3);
  RasterizeBuffer(face4);
  RasterizeBuffer(face5);
  RasterizeBuffer(face6);
  
  PrintBuffer();

  std::cout << " finished " << std::endl;
  return 0;
}

