#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <stdexcept>


#define loopi(start_l,end_l) for ( int i=start_l;i<end_l;++i )
#define loopi(start_l,end_l) for ( int i=start_l;i<end_l;++i )
#define loopj(start_l,end_l) for ( int j=start_l;j<end_l;++j )
#define loopk(start_l,end_l) for ( int k=start_l;k<end_l;++k )


struct SimplifyObject
{
	std::vector<double> vertices;
	std::vector<unsigned int> indices;
	std::vector<float> scalars;
};

struct vector3
{
	double x, y, z;
};

struct vec3f
{
	double x, y, z;

	inline vec3f(void) {}

	inline vec3f(vector3 a)
	{
		x = a.x; y = a.y; z = a.z;
	}

	inline vec3f(const double X, const double Y, const double Z)
	{
		x = X; y = Y; z = Z;
	}

	inline vec3f operator + (const vec3f& a) const
	{
		return vec3f(x + a.x, y + a.y, z + a.z);
	}

	inline vec3f operator += (const vec3f& a) const
	{
		return vec3f(x + a.x, y + a.y, z + a.z);
	}

	inline vec3f operator * (const double a) const
	{
		return vec3f(x * a, y * a, z * a);
	}

	inline vec3f operator * (const vec3f a) const
	{
		return vec3f(x * a.x, y * a.y, z * a.z);
	}

	inline vec3f v3() const
	{
		return vec3f(x, y, z);
	}

	inline vec3f operator = (const vector3 a)
	{
		x = a.x; y = a.y; z = a.z; return *this;
	}

	inline vec3f operator = (const vec3f a)
	{
		x = a.x; y = a.y; z = a.z; return *this;
	}

	inline vec3f operator / (const vec3f a) const
	{
		return vec3f(x / a.x, y / a.y, z / a.z);
	}

	inline vec3f operator - (const vec3f& a) const
	{
		return vec3f(x - a.x, y - a.y, z - a.z);
	}

	inline vec3f operator / (const double a) const
	{
		return vec3f(x / a, y / a, z / a);
	}

	inline double dot(const vec3f& a) const
	{
		return a.x * x + a.y * y + a.z * z;
	}

	inline vec3f cross(const vec3f& a, const vec3f& b)
	{
		x = a.y * b.z - a.z * b.y;
		y = a.z * b.x - a.x * b.z;
		z = a.x * b.y - a.y * b.x;
		return *this;
	}

	inline vec3f normalize(double desired_length = 1)
	{
		double square = sqrt(x * x + y * y + z * z);
		x /= square; y /= square; z /= square;

		return *this;
	}
};

vec3f barycentric(const vec3f& p, const vec3f& a, const vec3f& b, const vec3f& c);
vec3f interpolate(const vec3f& p, const vec3f& a, const vec3f& b, const vec3f& c, const vec3f attrs[3]);

struct SymetricMatrix
{

	public:

	SymetricMatrix(double c=0) { loopi(0,10) m[i] = c;  }

	SymetricMatrix(	double m11, double m12, double m13, double m14,
			            double m22, double m23, double m24,
			                        double m33, double m34,
			                                    double m44) {
			 m[0] = m11;  m[1] = m12;  m[2] = m13;  m[3] = m14;
			              m[4] = m22;  m[5] = m23;  m[6] = m24;
			                           m[7] = m33;  m[8] = m34;
			                                        m[9] = m44;
	}

	// Make plane

	SymetricMatrix(double a,double b,double c,double d)
	{
		m[0] = a*a;  m[1] = a*b;  m[2] = a*c;  m[3] = a*d;
		             m[4] = b*b;  m[5] = b*c;  m[6] = b*d;
		                          m[7 ] =c*c; m[8 ] = c*d;
		                                       m[9 ] = d*d;
	}

	double operator[](int c) const { return m[c]; }

	// Determinant

	double det(	int a11, int a12, int a13,
				int a21, int a22, int a23,
				int a31, int a32, int a33)
	{
		double det =  m[a11]*m[a22]*m[a33] + m[a13]*m[a21]*m[a32] + m[a12]*m[a23]*m[a31]
					- m[a13]*m[a22]*m[a31] - m[a11]*m[a23]*m[a32]- m[a12]*m[a21]*m[a33];
		return det;
	}

	const SymetricMatrix operator+(const SymetricMatrix& n) const
	{
		return SymetricMatrix( m[0]+n[0],   m[1]+n[1],   m[2]+n[2],   m[3]+n[3],
						                    m[4]+n[4],   m[5]+n[5],   m[6]+n[6],
						                                 m[ 7]+n[ 7], m[ 8]+n[8 ],
						                                              m[ 9]+n[9 ]);
	}

	SymetricMatrix& operator+=(const SymetricMatrix& n)
	{
		 m[0]+=n[0];   m[1]+=n[1];   m[2]+=n[2];   m[3]+=n[3];
		 m[4]+=n[4];   m[5]+=n[5];   m[6]+=n[6];   m[7]+=n[7];
		 m[8]+=n[8];   m[9]+=n[9];
		return *this;
	}

	double m[10];
};

enum class Attributes 
{
	NONE,
	NORMAL = 2,
	TEXCOORD = 4,
	COLOR = 8
};

/**
 * Triangle: ���������ݽṹ
 * v[3]:	 ������������������ std::vector<Vertex> vertices �е�����
 * err[0-3]����ʾ�����εı� 0-1, 1-2, 2-3, ÿ�������� calculate_error ����õ������
 * err[4]:	 ��ʾ�������� error ����С error����Ϊ�������ε� error
 * deleted:  ��ʾ���������Ѿ���ɾ����
 * dirty��	 ��ʾ�ڴ���Ĺ����У�������������صĶ����Ѿ������� collapse
 * n:		 ������
 * uvs		 ��ʹ�� uv, �������� uvs[i].x ���Ա���ֵ scalar ���в�ֵ
 * material: �����Ĳ��������Ϣ
 **/
struct Triangle { int v[3]; double err[4]; int deleted, dirty; Attributes attr; vec3f n; vec3f uvs[3]; int material; };
/**
 * Vertex�� �������ݽṹ
 * p:		�����λ��
 * tstart:	Ref�ж�Ӧ�Ĺ�����������Ϣ����ʼλ��
 * tcount:	�ö�������������θ���
 * q:		�ö����Ӧ�� Quadric Error Matrix
 * border:	�жϸö����ǲ���λ�ڱ߽�
**/
struct Vertex { vec3f p; int tstart, tcount; SymetricMatrix q; int border; };
/**
 * Ref:		�����洢ÿ�������������������Ϣ
 * tid:		�����ε�id�����ڴ�triangles�п��ٻ�ȡ��Ӧ��������
 * tvertex:	��¼�ö����Ӧ�����������е��ĸ����㣬0,1,2
**/
struct Ref { int tid, tvertex; };

class Simplify
{
public:
	Simplify() {};
	~Simplify() {};
	SimplifyObject simplify(const std::vector<double>& vertices, const std::vector<unsigned int>& indices, const std::vector<float>& scalars, 
		double reduceFraction = 0.5, double agressiveness = 7);

	SimplifyObject simplify(const std::vector<double>& vertices, const std::vector<unsigned int>& indices, 
		double reduceFraction = 0.5, double agressiveness = 7);


private:
	std::vector<Triangle> triangles;
	std::vector<Vertex> vertices;
	std::vector<Ref> refs;


	double vertex_error(SymetricMatrix q, double x, double y, double z);
	double calculate_error(int id_v1, int id_v2, vec3f& p_result);
	bool flipped(vec3f p, int i0, int i1, Vertex& v0, Vertex& v1, std::vector<int>& deleted);
	void update_uvs(int i0, const Vertex& v, const vec3f& p, std::vector<int>& deleted);
	void update_triangles(int i0, Vertex& v, std::vector<int>& deleted, int& deleted_triangles);
	void update_mesh(int iteration);
	void compact_mesh();
	void simplify_mesh(int target_count, double agressiveness, bool verbose);
	void load_triangle(const std::vector<double>& in_vertices, const std::vector<unsigned int>& in_indices, const std::vector<float>& in_scalars);
	SimplifyObject write_triangle();


};
