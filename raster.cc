//
// Rasterer
//
// Anthony Mesa
//

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

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
		buffer = new unsigned int[buff_size]();
	}

	~Buffer()
	{
		delete[](buffer);
	}

	void Clear()
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < width; j++)
			{
				buffer[(i * width) + j] = 0;
			}
		}
	}

	void Print()
	{
		for (int k = 0; k < width; k++)
		{
			for (int l = 0; l < width; l++)
			{
				std::cout << buffer[(k * width) + l] << " ";
			}
			std::cout << std::endl;
		}
	}

	void Set(int x, int y, unsigned int value)
	{
		std::cout << x << " " << y << std::endl;
		buffer[(y * width) + x] = value;
	}
};

namespace MuseRaster
{
Buffer *local_buffer;

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
	if ((*face_data)[1] > (*face_data)[4])
	{
		std::swap((*face_data)[0], (*face_data)[3]);
		std::swap((*face_data)[1], (*face_data)[4]);
		std::swap((*face_data)[2], (*face_data)[5]);
	}

	if ((*face_data)[4] > (*face_data)[7])
	{
		std::swap((*face_data)[3], (*face_data)[6]);
		std::swap((*face_data)[4], (*face_data)[7]);
		std::swap((*face_data)[5], (*face_data)[8]);
	}

	if ((*face_data)[1] > (*face_data)[4])
	{
		std::swap((*face_data)[0], (*face_data)[3]);
		std::swap((*face_data)[1], (*face_data)[4]);
		std::swap((*face_data)[2], (*face_data)[5]);
	}
}

float Distance(Point *_u, Point *_v)
{
	float square_x = std::pow((_u->x - _v->x), 2);
	float square_y = std::pow((_u->y - _v->y), 2);
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
	return v->z + ((w->z - v->z) * (Distance(u, v) / Distance(w, v)));
}

void GetFaceFromVector(
	std::vector<unsigned int> *face,
	const std::vector<unsigned int> *faces,
	const int face_index)
{
	for (int i = 0; i < 9; i++)
	{
		face->push_back((*faces)[(face_index * 9) + i]);
	}

	OrderFace(face);
}

void RasterizeLine(int omega, Point *A, Point *B)
{
	if (A->x > B->x)
	{
		std::swap(A, B);
	}
	std::cout << "Rasterizing new line..." << std::endl;

	for (int i = A->x; i < B->x; i++)
	{
		Point *T = new Point(i, omega, 0);
		T->z = Interpolate(T, A, B);
		local_buffer->Set(T->x, T->y, T->z);
		delete(T);
	}
}

void RasterizeTop(Point *L, Point *M, Point *H)
{
	std::cout << "Rasterizing Top..." << std::endl;

	float m_a = Slope(H, L);
	float b_a = YIntercept(m_a, H);

	float m_b = Slope(H, M);
	float b_b = YIntercept(m_b, H);

	Point *A = new Point(0, 0, 0);
	Point *B = new Point(0, 0, 0);

	for (int omega = M->y; omega < H->y; omega++)
	{
		A->y = omega;
		A->x = !std::isinf(m_a) ? XIntercept(omega, b_a, m_a) : H->x;
		A->z = Interpolate(A, H, L);

		B->y = omega;
		B->x = !std::isinf(m_b) ? XIntercept(omega, b_b, m_b) : M->x;
		B->z = Interpolate(B, H, M);

		RasterizeLine(omega, A, B);
	}
	
	delete (A);
	delete (B);
}

void RasterizeBottom(Point *L, Point *M, Point *H)
{
	std::cout << "Rasterizing Bottom..." << std::endl;

	Point *A = new Point(0, 0, 0);
	Point *B = new Point(0, 0, 0);

	float m_a = Slope(H, L);
	float b_a = YIntercept(m_a, H);

	float m_b = Slope(L, M);
	float b_b = YIntercept(m_b, L);

	for (int omega = M->y; omega > L->y; omega--)
	{
		A->y = omega;
		A->x = !std::isinf(m_a) ? XIntercept(omega, b_a, m_a) : L->x;
		A->z = Interpolate(A, H, L);

		B->y = omega;
		B->x = !std::isinf(m_b) ? XIntercept(omega, b_b, m_b) : M->x;
		B->z = Interpolate(B, L, M);

		RasterizeLine(omega, A, B);
	}

	delete (A);
	delete (B);
}

void RasterizeFace(std::vector<unsigned int> *face)
{
	std::cout << "Rasterizing new face..." << std::endl;

	Point *L = new Point((*face)[0], (*face)[1], (*face)[2]);
	Point *M = new Point((*face)[3], (*face)[4], (*face)[5]);
	Point *H = new Point((*face)[6], (*face)[7], (*face)[8]);

	RasterizeTop(L, M, H);
	RasterizeBottom(L, M, H);

	delete (L);
	delete (M);
	delete (H);
}

void RasterizeDataToBuffer(const std::vector<unsigned int> *faces, Buffer *audio_buffer)
{
	local_buffer = audio_buffer;
	int faces_amt = faces->size() / 9;

	for (int face_index = 0; face_index < faces_amt; face_index++)
	{
		std::vector<unsigned int> *face = new std::vector<unsigned int>();

		GetFaceFromVector(face, faces, face_index);

		RasterizeFace(face);

		delete (face);
	}
}
};

int main()
{
	std::cout << "starting..." << std::endl;

	std::vector<unsigned int> faces = {
		0, 0, 9, 5, 15, 1, 0, 19, 6,
		0, 0, 9, 5, 15, 1, 14, 3, 5,
		0, 0, 9, 14, 3, 5, 19, 0, 3,
		5, 15, 1, 14, 3, 5, 19, 19, 2,
		19, 0, 3, 14, 3, 5, 19, 19, 2,
		0, 19, 6, 5, 15, 1, 19, 19, 2};

	Buffer *audio_buffer = new Buffer(20);
	
	MuseRaster::RasterizeDataToBuffer(&faces, audio_buffer);

	std::cout << "Raster complete..." << std::endl;

	audio_buffer->Print();

	delete(audio_buffer);

	std::cout << "finished. " << std::endl;
	return 0;
}
