///////////////////////////////////////////////
////
//// Mesh Simplification Tutorial
////
//// (C) by Sven Forstmann in 2014
////
//// License : MIT
//// http://opensource.org/licenses/MIT
////
////https://github.com/sp4cerat/Fast-Quadric-Mesh-Simplification
////
//// 5/2016: Chris Rorden created minimal version for OSX/Linux/Windows compile
//
//#include <string.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <map>
//#include <vector>
//#include <string>
//#include <math.h>
//#include <time.h>
//#include <stdexcept>
//
//
//#define loopi(start_l,end_l) for ( int i=start_l;i<end_l;++i )
//#define loopi(start_l,end_l) for ( int i=start_l;i<end_l;++i )
//#define loopj(start_l,end_l) for ( int j=start_l;j<end_l;++j )
//#define loopk(start_l,end_l) for ( int k=start_l;k<end_l;++k )
//
//
//struct SimplifyObject
//{
//	std::vector<double> vertices;
//	std::vector<unsigned int> indices;
//	std::vector<float> scalars;
//};
//
//
//struct vector3
//{
//double x, y, z;
//};
//
//struct vec3f
//{
//    double x, y, z;
//
//    inline vec3f( void ) {}
//
//    //inline vec3f operator =( vector3 a )
//	// { vec3f b ; b.x = a.x; b.y = a.y; b.z = a.z; return b;}
//
//    inline vec3f( vector3 a )
//	 { x = a.x; y = a.y; z = a.z; }
//
//    inline vec3f( const double X, const double Y, const double Z )
//    { x = X; y = Y; z = Z; }
//
//    inline vec3f operator + ( const vec3f& a ) const
//    { return vec3f( x + a.x, y + a.y, z + a.z ); }
//
//	inline vec3f operator += ( const vec3f& a ) const
//    { return vec3f( x + a.x, y + a.y, z + a.z ); }
//
//    inline vec3f operator * ( const double a ) const
//    { return vec3f( x * a, y * a, z * a ); }
//
//    inline vec3f operator * ( const vec3f a ) const
//    { return vec3f( x * a.x, y * a.y, z * a.z ); }
//
//    inline vec3f v3 () const
//    { return vec3f( x , y, z ); }
//
//    inline vec3f operator = ( const vector3 a )
//    { x=a.x;y=a.y;z=a.z;return *this; }
//
//    inline vec3f operator = ( const vec3f a )
//    { x=a.x;y=a.y;z=a.z;return *this; }
//
//    inline vec3f operator / ( const vec3f a ) const
//    { return vec3f( x / a.x, y / a.y, z / a.z ); }
//
//    inline vec3f operator - ( const vec3f& a ) const
//    { return vec3f( x - a.x, y - a.y, z - a.z ); }
//
//    inline vec3f operator / ( const double a ) const
//    { return vec3f( x / a, y / a, z / a ); }
//
//    inline double dot( const vec3f& a ) const
//    { return a.x*x + a.y*y + a.z*z; }
//
//    inline vec3f cross( const vec3f& a , const vec3f& b )
//    {
//		x = a.y * b.z - a.z * b.y;
//		y = a.z * b.x - a.x * b.z;
//		z = a.x * b.y - a.y * b.x;
//		return *this;
//	}
//
//    inline double angle( const vec3f& v )
//    {
//		vec3f a = v , b = *this;
//		double dot = v.x*x + v.y*y + v.z*z;
//		double len = a.length() * b.length();
//		if(len==0)len=0.00001f;
//		double input = dot  / len;
//		if (input<-1) input=-1;
//		if (input>1) input=1;
//		return (double) acos ( input );
//	}
//
//    inline double angle2( const vec3f& v , const vec3f& w )
//    {
//		vec3f a = v , b= *this;
//		double dot = a.x*b.x + a.y*b.y + a.z*b.z;
//		double len = a.length() * b.length();
//		if(len==0)len=1;
//
//		vec3f plane; plane.cross( b,w );
//
//		if ( plane.x * a.x + plane.y * a.y + plane.z * a.z > 0 )
//			return (double) -acos ( dot  / len );
//
//		return (double) acos ( dot  / len );
//	}
//
//    inline vec3f rot_x( double a )
//    {
//		double yy = cos ( a ) * y + sin ( a ) * z;
//		double zz = cos ( a ) * z - sin ( a ) * y;
//		y = yy; z = zz;
//		return *this;
//	}
//    inline vec3f rot_y( double a )
//    {
//		double xx = cos ( -a ) * x + sin ( -a ) * z;
//		double zz = cos ( -a ) * z - sin ( -a ) * x;
//		x = xx; z = zz;
//		return *this;
//	}
//    inline void clamp( double min, double max )
//    {
//		if (x<min) x=min;
//		if (y<min) y=min;
//		if (z<min) z=min;
//		if (x>max) x=max;
//		if (y>max) y=max;
//		if (z>max) z=max;
//	}
//    inline vec3f rot_z( double a )
//    {
//		double yy = cos ( a ) * y + sin ( a ) * x;
//		double xx = cos ( a ) * x - sin ( a ) * y;
//		y = yy; x = xx;
//		return *this;
//	}
//    inline vec3f invert()
//	{
//		x=-x;y=-y;z=-z;return *this;
//	}
//    inline vec3f frac()
//	{
//		return vec3f(
//			x-double(int(x)),
//			y-double(int(y)),
//			z-double(int(z))
//			);
//	}
//
//    inline vec3f integer()
//	{
//		return vec3f(
//			double(int(x)),
//			double(int(y)),
//			double(int(z))
//			);
//	}
//
//    inline double length() const
//    {
//		return (double)sqrt(x*x + y*y + z*z);
//	}
//
//    inline vec3f normalize( double desired_length = 1 )
//    {
//		double square = sqrt(x*x + y*y + z*z);
//		/*
//		if (square <= 0.00001f )
//		{
//			x=1;y=0;z=0;
//			return *this;
//		}*/
//		//double len = desired_length / square;
//		x/=square;y/=square;z/=square;
//
//		return *this;
//	}
//    static vec3f normalize( vec3f a );
//
//	static void random_init();
//	static double random_double();
//	static vec3f random();
//
//	static int random_number;
//
//	double random_double_01(double a){
//		double rnf=a*14.434252+a*364.2343+a*4213.45352+a*2341.43255+a*254341.43535+a*223454341.3523534245+23453.423412;
//		int rni=((int)rnf)%100000;
//		return double(rni)/(100000.0f-1.0f);
//	}
//
//	vec3f random01_fxyz(){
//		x=(double)random_double_01(x);
//		y=(double)random_double_01(y);
//		z=(double)random_double_01(z);
//		return *this;
//	}
//
//};
//
//vec3f barycentric(const vec3f &p, const vec3f &a, const vec3f &b, const vec3f &c){
//	vec3f v0 = b-a;
//	vec3f v1 = c-a;
//	vec3f v2 = p-a;
//	double d00 = v0.dot(v0);
//	double d01 = v0.dot(v1);
//	double d11 = v1.dot(v1);
//	double d20 = v2.dot(v0);
//	double d21 = v2.dot(v1);
//	double denom = d00*d11-d01*d01;
//	double v = (d11 * d20 - d01 * d21) / denom;
//	double w = (d00 * d21 - d01 * d20) / denom;
//	double u = 1.0 - v - w;
//	return vec3f(u,v,w);
//}
//
//vec3f interpolate(const vec3f &p, const vec3f &a, const vec3f &b, const vec3f &c, const vec3f attrs[3])
//{
//	vec3f bary = barycentric(p,a,b,c);
//	vec3f out = vec3f(0,0,0);
//	out = out + attrs[0] * bary.x;
//	out = out + attrs[1] * bary.y;
//	out = out + attrs[2] * bary.z;
//	return out;
//}
//
//double min(double v1, double v2) {
//	return fmin(v1,v2);
//}
//
//
//class SymetricMatrix {
//
//	public:
//
//	// Constructor
//
//	SymetricMatrix(double c=0) { loopi(0,10) m[i] = c;  }
//
//	SymetricMatrix(	double m11, double m12, double m13, double m14,
//			            double m22, double m23, double m24,
//			                        double m33, double m34,
//			                                    double m44) {
//			 m[0] = m11;  m[1] = m12;  m[2] = m13;  m[3] = m14;
//			              m[4] = m22;  m[5] = m23;  m[6] = m24;
//			                           m[7] = m33;  m[8] = m34;
//			                                        m[9] = m44;
//	}
//
//	// Make plane
//
//	SymetricMatrix(double a,double b,double c,double d)
//	{
//		m[0] = a*a;  m[1] = a*b;  m[2] = a*c;  m[3] = a*d;
//		             m[4] = b*b;  m[5] = b*c;  m[6] = b*d;
//		                          m[7 ] =c*c; m[8 ] = c*d;
//		                                       m[9 ] = d*d;
//	}
//
//	double operator[](int c) const { return m[c]; }
//
//	// Determinant
//
//	double det(	int a11, int a12, int a13,
//				int a21, int a22, int a23,
//				int a31, int a32, int a33)
//	{
//		double det =  m[a11]*m[a22]*m[a33] + m[a13]*m[a21]*m[a32] + m[a12]*m[a23]*m[a31]
//					- m[a13]*m[a22]*m[a31] - m[a11]*m[a23]*m[a32]- m[a12]*m[a21]*m[a33];
//		return det;
//	}
//
//	const SymetricMatrix operator+(const SymetricMatrix& n) const
//	{
//		return SymetricMatrix( m[0]+n[0],   m[1]+n[1],   m[2]+n[2],   m[3]+n[3],
//						                    m[4]+n[4],   m[5]+n[5],   m[6]+n[6],
//						                                 m[ 7]+n[ 7], m[ 8]+n[8 ],
//						                                              m[ 9]+n[9 ]);
//	}
//
//	SymetricMatrix& operator+=(const SymetricMatrix& n)
//	{
//		 m[0]+=n[0];   m[1]+=n[1];   m[2]+=n[2];   m[3]+=n[3];
//		 m[4]+=n[4];   m[5]+=n[5];   m[6]+=n[6];   m[7]+=n[7];
//		 m[8]+=n[8];   m[9]+=n[9];
//		return *this;
//	}
//
//	double m[10];
//};
/////////////////////////////////////////////
//
//namespace Simplify
//{
//	// Global Variables & Strctures
//	enum Attributes {
//		NONE,
//		NORMAL = 2,
//		TEXCOORD = 4,
//		COLOR = 8
//	};
//	/**
//	 * Triangle: 三角形数据结构
//	 * v[3]:	 是三角形三个顶点在 std::vector<Vertex> vertices 中的索引
//	 * err[0-3]：表示三角形的边 0-1, 1-2, 2-3, 每条边利用 calculate_error 计算得到的误差
//	 * err[4]:	 表示上面三个 error 的最小 error，作为该三角形的 error
//	 * deleted:  表示该三角形已经被删除；
//	 * dirty：	 表示在处理的过程中，该三角形有相关的顶点已经参与了 collapse
//	 * n:		 法向量
//	 * uvs		 不使用 uv, 而是利用 uvs[i].x 来对标量值 scalar 进行插值
//	 * material: 不关心材质相关信息
//	 **/
//	struct Triangle { int v[3]; double err[4]; int deleted, dirty, attr; vec3f n; vec3f uvs[3]; int material; };
//	/**
//	 * Vertex： 顶点数据结构
//	 * p:		顶点的位置
//	 * tstart:	Ref中对应的关联三角形信息的起始位置
//	 * tcount:	该顶点关联的三角形个数
//	 * q:		该顶点对应的 Quadric Error Matrix
//	 * border:	判断该顶点是不是位于边界
//	**/
//	struct Vertex { vec3f p; int tstart, tcount; SymetricMatrix q; int border; };
//	/**
//	 * Ref:		用来存储每个顶点关联的三角形信息
//	 * tid:		三角形的id，用于从triangles中快速获取对应的三角形
//	 * tvertex:	记录该顶点对应的是三角形中的哪个顶点，0,1,2
//	**/
//	struct Ref { int tid, tvertex; };
//	std::vector<Triangle> triangles;
//	std::vector<Vertex> vertices;
//	std::vector<Ref> refs;
//	std::string mtllib;
//	std::vector<std::string> materials;
//
//	// Helper functions
//
//	double vertex_error(SymetricMatrix q, double x, double y, double z);
//	double calculate_error(int id_v1, int id_v2, vec3f& p_result);
//	bool flipped(vec3f p, int i0, int i1, Vertex& v0, Vertex& v1, std::vector<int>& deleted);
//	void update_uvs(int i0, const Vertex& v, const vec3f& p, std::vector<int>& deleted);
//	void update_triangles(int i0, Vertex& v, std::vector<int>& deleted, int& deleted_triangles);
//	void update_mesh(int iteration);
//	void compact_mesh();
//	//
//	// 简化函数
//	//
//	// target_count  : 目标三角形个数
//	// agressiveness : sharpness to increase the threshold.
//	//                 5..8 are good numbers
//	//                 more iterations yield higher quality
//	//
//	void simplify_mesh(int target_count, double agressiveness = 8, bool verbose = 0)
//	{
//		// 1. 标注所有三角形的deleted tag为0
//		loopi(0, triangles.size())
//		{
//			triangles[i].deleted = 0;
//		}
//
//		// main iteration loop
//		int deleted_triangles = 0;
//		std::vector<int> deleted0, deleted1;
//		int triangle_count = triangles.size();
//		// 2. 迭代loop
//		for (int iteration = 0; iteration < 100; iteration++)
//		{
//			// 2.1 减少的三角形数量满足要求，则退出迭代
//			if (triangle_count - deleted_triangles <= target_count)break;
//
//			// 2.2 每个一定时间更新网格
//			// update_mesh处理的事情是，将三角形的存储更加紧凑
//			// 第一次调用的时候会初始化 Quadrics by Plane & Edge Errors
//			if (iteration % 5 == 0)
//			{
//				update_mesh(iteration);
//			}
//
//			// 2.3 清理三角形的dirty flag
//			loopi(0, triangles.size()) triangles[i].dirty = 0;
//
//			// 2.4 神秘公式，用于设定阈值
//			// 三角形中所有低于阈值的边会被删除
//			// 下面的参数对大多数模型效果好, 如果效果差可以调整 3 个参数
//			double threshold = 0.000000001 * pow(double(iteration + 3), agressiveness);
//
//			// target number of triangles reached ? Then break
//			if ((verbose) && (iteration % 5 == 0)) {
//				printf("iteration %d - triangles %d threshold %g\n", iteration, triangle_count - deleted_triangles, threshold);
//			}
//
//			// 2.5 删除顶点，并且标记删除的三角形
//			loopi(0, triangles.size())
//			{
//				Triangle& t = triangles[i];
//				if (t.err[3] > threshold) continue;
//				if (t.deleted) continue;
//				if (t.dirty) continue;
//
//				loopj(0, 3)if (t.err[j] < threshold)
//				{
//
//					int i0 = t.v[j]; Vertex& v0 = vertices[i0];
//					int i1 = t.v[(j + 1) % 3]; Vertex& v1 = vertices[i1];
//					// 略过一个点在边界上，一个点不在边界上的情况
//					if (v0.border != v1.border)  continue;
//
//					vec3f p;
//					// 计算 error 的同时, 计算坍缩后的 p 点的位置
//					calculate_error(i0, i1, p);
//					deleted0.resize(v0.tcount); // normals temporarily
//					deleted1.resize(v1.tcount); // normals temporarily
//					// 如果翻转, 则不删除
//					if (flipped(p, i0, i1, v0, v1, deleted0)) continue;
//
//					if (flipped(p, i1, i0, v1, v0, deleted1)) continue;
//
//					if ((t.attr & TEXCOORD) == TEXCOORD)
//					{
//						update_uvs(i0, v0, p, deleted0);
//						update_uvs(i0, v1, p, deleted1);
//					}
//
//					// not flipped, so remove edge
//					v0.p = p;
//					v0.q = v1.q + v0.q;
//					int tstart = refs.size();
//
//					// 删除三角形
//					update_triangles(i0, v0, deleted0, deleted_triangles);
//					update_triangles(i0, v1, deleted1, deleted_triangles);
//
//					int tcount = refs.size() - tstart;
//
//					if (tcount <= v0.tcount)
//					{
//						// 节省内存的操作
//						if (tcount)memcpy(&refs[v0.tstart], &refs[tstart], tcount * sizeof(Ref));
//					}
//					else
//						// append
//						v0.tstart = tstart;
//
//					v0.tcount = tcount;
//					break;
//				}
//				// done?
//				if (triangle_count - deleted_triangles <= target_count)break;
//			}
//		}
//		// 结束前删除网格
//		compact_mesh();
//	} //simplify_mesh()
//
//	// Check if a triangle flips when this edge is removed
//	bool flipped(vec3f p, int i0, int i1, Vertex& v0, Vertex& v1, std::vector<int>& deleted)
//	{
//
//		loopk(0, v0.tcount)
//		{
//			Triangle& t = triangles[refs[v0.tstart + k].tid];
//			if (t.deleted)continue;
//
//			int s = refs[v0.tstart + k].tvertex;
//			int id1 = t.v[(s + 1) % 3];
//			int id2 = t.v[(s + 2) % 3];
//
//			if (id1 == i1 || id2 == i1) // delete ?
//			{
//
//				deleted[k] = 1;
//				continue;
//			}
//			vec3f d1 = vertices[id1].p - p; d1.normalize();
//			vec3f d2 = vertices[id2].p - p; d2.normalize();
//			if (fabs(d1.dot(d2)) > 0.999) return true;
//			vec3f n;
//			n.cross(d1, d2);
//			n.normalize();
//			deleted[k] = 0;
//			if (n.dot(t.n) < 0.2) return true;
//		}
//		return false;
//	}
//
//	// update_uvs
//
//	void update_uvs(int i0, const Vertex& v, const vec3f& p, std::vector<int>& deleted)
//	{
//		loopk(0, v.tcount)
//		{
//			Ref& r = refs[v.tstart + k];
//			Triangle& t = triangles[r.tid];
//			if (t.deleted)continue;
//			if (deleted[k])continue;
//			vec3f p1 = vertices[t.v[0]].p;
//			vec3f p2 = vertices[t.v[1]].p;
//			vec3f p3 = vertices[t.v[2]].p;
//			t.uvs[r.tvertex] = interpolate(p, p1, p2, p3, t.uvs);
//		}
//	}
//
//	// 更新三角形的连接关系，以及边的 error
//
//	void update_triangles(int i0, Vertex& v, std::vector<int>& deleted, int& deleted_triangles)
//	{
//		vec3f p;
//		loopk(0, v.tcount)
//		{
//			Ref& r = refs[v.tstart + k];
//			Triangle& t = triangles[r.tid];
//			if (t.deleted)continue;
//			if (deleted[k])
//			{
//				t.deleted = 1;
//				deleted_triangles++;
//				continue;
//			}
//			t.v[r.tvertex] = i0;
//			t.dirty = 1;
//			t.err[0] = calculate_error(t.v[0], t.v[1], p);
//			t.err[1] = calculate_error(t.v[1], t.v[2], p);
//			t.err[2] = calculate_error(t.v[2], t.v[0], p);
//			t.err[3] = min(t.err[0], min(t.err[1], t.err[2]));
//			refs.push_back(r);
//		}
//	}
//
//	// compact triangles, compute edge error and build reference list
//
//	void update_mesh(int iteration)
//	{
//		if (iteration > 0) // compact triangles
//		{
//			int dst = 0;
//			loopi(0, triangles.size())
//				if (!triangles[i].deleted)
//				{
//					triangles[dst++] = triangles[i];
//				}
//			triangles.resize(dst);
//		}
//		//
//
//		// Init Reference ID list
//		loopi(0, vertices.size())
//		{
//			vertices[i].tstart = 0;
//			vertices[i].tcount = 0;
//		}
//		loopi(0, triangles.size())
//		{
//			Triangle& t = triangles[i];
//			loopj(0, 3) vertices[t.v[j]].tcount++;
//		}
//		int tstart = 0;
//		loopi(0, vertices.size())
//		{
//			Vertex& v = vertices[i];
//			v.tstart = tstart;
//			tstart += v.tcount;
//			v.tcount = 0;
//		}
//
//		// Write References
//		refs.resize(triangles.size() * 3);
//		loopi(0, triangles.size())
//		{
//			Triangle& t = triangles[i];
//			loopj(0, 3)
//			{
//				Vertex& v = vertices[t.v[j]];
//				refs[v.tstart + v.tcount].tid = i;
//				refs[v.tstart + v.tcount].tvertex = j;
//				v.tcount++;
//			}
//		}
//
//		// Init Quadrics by Plane & Edge Errors
//		//
//		// required at the beginning ( iteration == 0 )
//		// recomputing during the simplification is not required,
//		// but mostly improves the result for closed meshes
//		//
//		if (iteration == 0)
//		{
//			// Identify boundary : vertices[].border=0,1
//
//			std::vector<int> vcount, vids;
//
//			loopi(0, vertices.size())
//				vertices[i].border = 0;
//
//			loopi(0, vertices.size())
//			{
//				Vertex& v = vertices[i];
//				vcount.clear();
//				vids.clear();
//				loopj(0, v.tcount)
//				{
//					int k = refs[v.tstart + j].tid;
//					Triangle& t = triangles[k];
//					loopk(0, 3)
//					{
//						int ofs = 0, id = t.v[k];
//						while (ofs < vcount.size())
//						{
//							if (vids[ofs] == id)break;
//							ofs++;
//						}
//						if (ofs == vcount.size())
//						{
//							vcount.push_back(1);
//							vids.push_back(id);
//						}
//						else
//							vcount[ofs]++;
//					}
//				}
//				loopj(0, vcount.size()) if (vcount[j] == 1)
//					vertices[vids[j]].border = 1;
//			}
//			//initialize errors
//			loopi(0, vertices.size())
//				vertices[i].q = SymetricMatrix(0.0);
//
//			loopi(0, triangles.size())
//			{
//				Triangle& t = triangles[i];
//				vec3f n, p[3];
//				loopj(0, 3) p[j] = vertices[t.v[j]].p;
//				n.cross(p[1] - p[0], p[2] - p[0]);
//				n.normalize();
//				t.n = n;
//				loopj(0, 3) vertices[t.v[j]].q =
//					vertices[t.v[j]].q + SymetricMatrix(n.x, n.y, n.z, -n.dot(p[0]));
//			}
//			loopi(0, triangles.size())
//			{
//				// Calc Edge Error
//				Triangle& t = triangles[i]; vec3f p;
//				loopj(0, 3) t.err[j] = calculate_error(t.v[j], t.v[(j + 1) % 3], p);
//				t.err[3] = min(t.err[0], min(t.err[1], t.err[2]));
//			}
//		}
//	}
//
//	// Finally compact mesh before exiting
//	void compact_mesh()
//	{
//		int dst = 0;
//		loopi(0, vertices.size())
//		{
//			vertices[i].tcount = 0;
//		}
//		loopi(0, triangles.size())
//			if (!triangles[i].deleted)
//			{
//				Triangle& t = triangles[i];
//				triangles[dst++] = t;
//				loopj(0, 3)vertices[t.v[j]].tcount = 1;
//			}
//		triangles.resize(dst);
//		dst = 0;
//		loopi(0, vertices.size())
//			if (vertices[i].tcount)
//			{
//				vertices[i].tstart = dst;
//				vertices[dst].p = vertices[i].p;
//				dst++;
//			}
//		loopi(0, triangles.size())
//		{
//			Triangle& t = triangles[i];
//			loopj(0, 3)t.v[j] = vertices[t.v[j]].tstart;
//		}
//		vertices.resize(dst);
//	}
//
//	// Error between vertex and Quadric
//	double vertex_error(SymetricMatrix q, double x, double y, double z)
//	{
//		return   q[0] * x * x + 2 * q[1] * x * y + 2 * q[2] * x * z + 2 * q[3] * x + q[4] * y * y
//			+ 2 * q[5] * y * z + 2 * q[6] * y + q[7] * z * z + 2 * q[8] * z + q[9];
//	}
//
//	// Error for one edge
//	double calculate_error(int id_v1, int id_v2, vec3f& p_result)
//	{
//		// compute interpolated vertex
//		SymetricMatrix q = vertices[id_v1].q + vertices[id_v2].q;
//		bool   border = vertices[id_v1].border & vertices[id_v2].border;
//		double error = 0;
//		double det = q.det(0, 1, 2, 1, 4, 5, 2, 5, 7);
//		if (det != 0 && !border)
//		{
//
//			// q_delta is invertible
//			p_result.x = -1 / det * (q.det(1, 2, 3, 4, 5, 6, 5, 7, 8));	// vx = A41/det(q_delta)
//			p_result.y = 1 / det * (q.det(0, 2, 3, 1, 5, 6, 2, 7, 8));	// vy = A42/det(q_delta)
//			p_result.z = -1 / det * (q.det(0, 1, 3, 1, 4, 6, 2, 5, 8));	// vz = A43/det(q_delta)
//
//			error = vertex_error(q, p_result.x, p_result.y, p_result.z);
//		}
//		else
//		{
//			// det = 0 -> try to find best result
//			vec3f p1 = vertices[id_v1].p;
//			vec3f p2 = vertices[id_v2].p;
//			vec3f p3 = (p1 + p2) / 2;
//			double error1 = vertex_error(q, p1.x, p1.y, p1.z);
//			double error2 = vertex_error(q, p2.x, p2.y, p2.z);
//			double error3 = vertex_error(q, p3.x, p3.y, p3.z);
//			error = min(error1, min(error2, error3));
//			if (error1 == error) p_result = p1;
//			if (error2 == error) p_result = p2;
//			if (error3 == error) p_result = p3;
//		}
//		return error;
//	}
//
//
//	void load_triangle(const std::vector<double>& in_vertices, const std::vector<unsigned int>& in_indices, const std::vector<float>& in_scalars)
//	{
//		vertices.clear();
//		triangles.clear();
//
//		for (size_t i = 0; i < in_vertices.size(); i += 3)
//		{
//			Vertex v;
//			v.p.x = in_vertices[i + 0];
//			v.p.y = in_vertices[i + 1];
//			v.p.z = in_vertices[i + 2];
//			vertices.emplace_back(v);
//		}
//		int material = -1;
//
//		for (size_t i = 0; i < in_indices.size(); i += 3)
//		{
//			Triangle t;
//			for (int j = 0; j < 3; j++)
//			{
//				t.v[j] = in_indices[i + j];
//				t.uvs[j].x = in_scalars[in_indices[i + j]];    // 利用 uvs[i].x 对标量进行插值
//				t.uvs[j].y = 0;
//				t.uvs[j].z = 0;
//			}
//			t.attr = TEXCOORD;
//
//			t.material = material;
//			triangles.emplace_back(t);
//		}
//	}
//
//	SimplifyObject write_triangle()
//	{
//		std::vector<double> out_vertices;
//		std::vector<unsigned int> out_indices;
//		std::vector<float> out_scalars;
//		for (size_t i = 0; i < vertices.size(); i++)
//		{
//			out_vertices.emplace_back(vertices[i].p.x);
//			out_vertices.emplace_back(vertices[i].p.y);
//			out_vertices.emplace_back(vertices[i].p.z);
//		}
//		out_scalars.resize(out_vertices.size());
//		for (auto t : triangles)
//		{
//			out_scalars[t.v[0]] = t.uvs[0].x;
//			out_scalars[t.v[1]] = t.uvs[1].x;
//			out_scalars[t.v[2]] = t.uvs[2].x;
//		}
//		for (auto t : triangles)
//		{
//			unsigned int v0 = t.v[0];
//			unsigned int v1 = t.v[1];
//			unsigned int v2 = t.v[2];
//			out_indices.emplace_back(v0);
//			out_indices.emplace_back(v1);
//			out_indices.emplace_back(v2);
//		}
//		return { out_vertices, out_indices, out_scalars };
//	}
//
//	/**
//	 * @brif 简化三角形调用函数
//	 * @return SimplifyObject, 结构体, 包含简化后的顶点、索引和标量向量
//	 * @param vertices, 待简化顶点
//	 * @param indices, 待简化索引
//	 * @param scalars, 待简化标量
//	 * @param reduceFraction, 三角形数量减少率, 默认为 0.5 (减半)
//	 * @param agressiveness, 阈值参数, 取值为 5-8 时较好
//	**/
//	SimplifyObject start(const std::vector<double>& vertices, const std::vector<unsigned int>& indices, const std::vector<float>& scalars,
//		double reduceFraction = 0.5, double agressiveness = 7.0)
//	{
//		load_triangle(vertices, indices, scalars);
//		if ((Simplify::triangles.size() < 3) || (Simplify::vertices.size() < 3))
//			throw std::runtime_error("can't make triangle");
//		if (reduceFraction <= 0.0 || reduceFraction > 1.0)
//		{
//			throw std::runtime_error("Ratio must be BETWEEN zero and one.");
//		}
//		int target_count = round((float)Simplify::triangles.size() * reduceFraction);
//		if (target_count < 4)
//		{
//			throw std::runtime_error("Object will not survive such extreme decimation");
//		}
//		clock_t start = clock();
//		printf("Input: %zu vertices, %zu triangles (target %d)\n", Simplify::vertices.size(), Simplify::triangles.size(), target_count);
//		int startSize = triangles.size();
//		simplify_mesh(target_count, agressiveness);
//		if (triangles.size() >= startSize) {
//			throw std::runtime_error("Unable to reduce mesh.\n");
//		}
//		SimplifyObject obj;
//		obj = write_triangle();
//		printf("Output: %zu vertices, %zu triangles (%f reduction; %.4f sec)\n", Simplify::vertices.size(), Simplify::triangles.size()
//			, (float)Simplify::triangles.size() / (float)startSize, ((float)(clock() - start)) / CLOCKS_PER_SEC);
//		return obj;
//	}
//	SimplifyObject start(const std::vector<double>& vertices, const std::vector<unsigned int>& indices, 
//		double reduceFraction = 0.5, double agressiveness = 7.0)
//	{
//		std::vector<float> temp(vertices.size() / 3, 0);
//		return start(vertices, indices, temp, reduceFraction, agressiveness);
//	}
//}
/////////////////////////////////////////////