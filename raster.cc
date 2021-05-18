//
// Rasterer
//
// Anthony Mesa
//
  
#include<iostream>  
#include<vector>
#include<cmath>
#include<algorithm>

class Buffer
{
private:
  unsigned int *buffer;
  int width;

public:
  Buffer(const int _width)
  {
    width = _width;
    const int buff_size = width * width;
    buffer = new unsigned int[buff_size];
  }

  void Clear()
  {
    for(int i = 0; i < width; i++)
    {
      for(int j = 0; j < width; j++)
      {
        buffer[(i * width) + j] = 0;
      }
    }
  }

  void Print()
  {
    for(int k = 0; k < width; k++)
    {
      for(int l = 0; l < width; l++)
      {
        std::cout << buffer[(k * width) + l] << " ";
      }
      std::cout << std::endl;
    }
  }

  void Set(int x, int y, unsigned int value)
  {
    buffer[(y * width) + x] = value;
  }
};

namespace MuseRaster
{
Buffer *audio_buffer;

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

  static void ToString(Point *_p)
  {
    std::cout << _p->x << " " << _p->y << " " << _p->z << std::endl;	
  }
};

void OrderFace(std::vector<unsigned int> *face_data)
{
  if ((*face_data)[1] > (*face_data)[4]){
    std::swap((*face_data)[0], (*face_data)[3]);
    std::swap((*face_data)[1],(*face_data)[4]);
    std::swap((*face_data)[2],(*face_data)[5]);
  }

  if ((*face_data)[4] > (*face_data)[7]){
    std::swap((*face_data)[3],(*face_data)[6]);
    std::swap((*face_data)[4],(*face_data)[7]);
    std::swap((*face_data)[5],(*face_data)[8]);
  }
 
  if ((*face_data)[1] > (*face_data)[4]){
    std::swap((*face_data)[0],(*face_data)[3]);
    std::swap((*face_data)[1],(*face_data)[4]);
    std::swap((*face_data)[2],(*face_data)[5]);
  }
}

float Distance(Point *_u, Point *_v)
{
  float square_x = std::pow((_u->x - _v->x), 2);
  float square_y = std::pow((_u->y - _v->x), 2);
  return std::sqrt(square_x + square_y);
}

float Slope(Point *_u, Point *_v)
{
  return (_u->y - _v->y) / (_u->x - _v->x);
}

float YIntercept(float _m, Point *_u)
{
  return _u->y - (_m * _u->x);
}

float XIntercept(int _y, float _b, float _m)
{
  return (_y - _b) / _m;
}

float Interpolate(Point *u, Point *v, Point *w)
{
        std::cout << "dha " << Distance(u, v) << std::endl;
    std::cout << "dhl " << Distance(w, v) << std::endl;
        std::cout << "m " << (Distance(u, v) / Distance(w, v)) << std::endl;
        std::cout << "interpolate " << v->z + ((w->z - v->z) * (Distance(u, v) / Distance(w, v))) << std::endl;
  return v->z + ((w->z - v->z) * (Distance(u, v) / Distance(w, v)));
}

void GetFaceFromVector(
  std::vector<unsigned int> *face, 
  const std::vector<unsigned int> *faces,
  const int face_index)
{
  for(int i = 0; i < 9; i++)
  {
    face->push_back((*faces)[face_index + i]);
  }

  OrderFace(face);
}

void RasterizeLine(int omega, Point *A, Point *B)
{
  if(A->x > B->x){ std::swap(A, B); }

  for(int i = A->x; i < (B->x ); i++)
  {
    Point *T = new Point(i, omega, 0);
    T->z = Interpolate(T, A, B);

    std::cout << T->z << std::endl;

    audio_buffer->Set(i, omega, T->z);
  }
}

void RasterizeTop(Point *L, Point *M, Point *H)
{
  float m_a = Slope(H, L);

  std::cout << "slope " << m_a << std::endl;
  float b_a = YIntercept(m_a, H);

  float m_b = Slope(H, M);
  float b_b = YIntercept(m_b, H);

  Point *A = new Point(0, 0, 0);
  Point *B = new Point(0, 0, 0); 

  for(int omega = M->y; omega < H->y; omega++)
  {
    A->y = omega;
    A->x = !std::isinf(m_a) ? XIntercept(omega, b_a, m_a) : H->x;
    std::cout << "A-x " << A->x << std::endl;
    A->z = Interpolate(A, H, L);
        std::cout << "A-z " << A->z << std::endl;

    B->y = omega;
    B->x = !std::isinf(m_b) ? XIntercept(omega, b_b, m_b) : M->x;
    std::cout << "B-x " << B->x << std::endl;
    B->z = Interpolate(B, H, M);
        std::cout << "B-z " << B->z << std::endl;

    RasterizeLine(omega, A, B);

  }
      delete(A);
    delete(B);
}

void RasterizeBottom(Point *L, Point *M, Point *H)
{
  Point *A = new Point(0, 0, 0);
  Point *B = new Point(0, 0, 0); 

  float m_a = Slope(H, L);
  float b_a = YIntercept(m_a, H);

  float m_b = Slope(L, M);
  float b_b = YIntercept(m_b, L);

  for(int omega = M->y; omega > L->y; omega--)
  {
    A->y = omega;
    A->x = !std::isinf(m_a) ? XIntercept(omega, b_a, m_a) : L->x;
    A->z = Interpolate(A, H, L);

    B->y = omega;
    B->x = !std::isinf(m_b) ? XIntercept(omega, b_b, m_b) : M->x;
    B->z = Interpolate(B, L, M);

    RasterizeLine(omega, A, B);
  }

    delete(A);
    delete(B);
}


void RasterizeFace(std::vector<unsigned int> *face)
{
  Point *L = new Point((*face)[0], (*face)[1], (*face)[2]);	
  Point *M = new Point((*face)[3], (*face)[4], (*face)[5]);
  Point *H = new Point((*face)[6], (*face)[7], (*face)[8]);

    Point::ToString(H);
Point::ToString(M);
Point::ToString(L);

  RasterizeTop(L, M, H);
  RasterizeBottom(L, M, H);

  delete(L);
  delete(M);
  delete(H);
}


Buffer *RasterizeData(const std::vector<unsigned int> *faces)
{
  audio_buffer = new Buffer(20);

  int faces_amt = faces->size() / 9;
  
  for(int face_index = 0; face_index < faces_amt; face_index += 9)
  {
    std::vector<unsigned int> *face = new std::vector<unsigned int>();

    GetFaceFromVector(face, faces, face_index);

    for(int i=0; i < face->size(); i++){
        std::cout << face->at(i) << ' ';
    }
    std::cout << std::endl;
    
    RasterizeFace(face);

    // does it need cleared?
    delete(face);
  }

  return audio_buffer;
}

};


int main()
{
    std::cout << "starting" << std::endl;

  std::vector<unsigned int> faces = { 
      0, 0, 9, 5, 15, 1, 0, 20, 6,
      0, 0, 9, 5, 15, 1, 14, 3, 5,
      0, 0, 9, 14, 3, 5, 20, 0, 3,
      5, 15, 1, 14, 3, 5, 20, 20, 2,
      20, 0, 3, 14, 3, 5, 20, 20, 2,
      0, 20, 6, 5, 15, 1, 20, 20, 2
    };

  Buffer *new_buffer = MuseRaster::RasterizeData(&faces);
  new_buffer->Print();

    delete(new_buffer);

  std::cout << " finished " << std::endl;
  return 0;
}

