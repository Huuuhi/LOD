#include "Simplify.h"

vec3f barycentric(const vec3f& p, const vec3f& a, const vec3f& b, const vec3f& c) {
	vec3f v0 = b - a;
	vec3f v1 = c - a;
	vec3f v2 = p - a;
	double d00 = v0.dot(v0);
	double d01 = v0.dot(v1);
	double d11 = v1.dot(v1);
	double d20 = v2.dot(v0);
	double d21 = v2.dot(v1);
	double denom = d00 * d11 - d01 * d01;
	double v = (d11 * d20 - d01 * d21) / denom;
	double w = (d00 * d21 - d01 * d20) / denom;
	double u = 1.0 - v - w;
	return vec3f(u, v, w);
}

vec3f interpolate(const vec3f& p, const vec3f& a, const vec3f& b, const vec3f& c, const vec3f attrs[3])
{
	vec3f bary = barycentric(p, a, b, c);
	vec3f out = vec3f(0, 0, 0);
	out = out + attrs[0] * bary.x;
	out = out + attrs[1] * bary.y;
	out = out + attrs[2] * bary.z;
	return out;
}


//
// 简化函数
//
// target_count  : 目标三角形个数
// agressiveness : sharpness to increase the threshold.
//                 5..8 are good numbers
//                 more iterations yield higher quality
// verbose		 : 为 true 则打印详细信息
void Simplify::simplify_mesh(int target_count, double agressiveness = 8, bool verbose = false)
{
	// 1. 标注所有三角形的deleted tag为0
	loopi(0, triangles.size())
	{
		triangles[i].deleted = 0;
	}

	// main iteration loop
	int deleted_triangles = 0;
	std::vector<int> deleted0, deleted1;
	int triangle_count = triangles.size();
	// 2. 迭代loop
	for (int iteration = 0; iteration < 100; iteration++)
	{
		// 2.1 减少的三角形数量满足要求，则退出迭代
		if (triangle_count - deleted_triangles <= target_count)break;

		// 2.2 每个一定时间更新网格
		// update_mesh处理的事情是，将三角形的存储更加紧凑
		// 第一次调用的时候会初始化 Quadrics by Plane & Edge Errors
		if (iteration % 5 == 0)
		{
			update_mesh(iteration);
		}

		// 2.3 清理三角形的dirty flag
		loopi(0, triangles.size()) triangles[i].dirty = 0;

		// 2.4 神秘公式，用于设定阈值
		// 三角形中所有低于阈值的边会被删除
		// 下面的参数对大多数模型效果好, 如果效果差可以调整 3 个参数
		double threshold = 0.000000001 * pow(double(iteration + 3), agressiveness);

		// target number of triangles reached ? Then break
		if ((verbose) && (iteration % 5 == 0)) {
			printf("iteration %d - triangles %d threshold %g\n", iteration, triangle_count - deleted_triangles, threshold);
		}

		// 2.5 删除顶点，并且标记删除的三角形
		loopi(0, triangles.size())
		{
			Triangle& t = triangles[i];
			if (t.err[3] > threshold) continue;
			if (t.deleted) continue;
			if (t.dirty) continue;

			loopj(0, 3)if (t.err[j] < threshold)
			{

				int i0 = t.v[j]; Vertex& v0 = vertices[i0];
				int i1 = t.v[(j + 1) % 3]; Vertex& v1 = vertices[i1];
				// 略过一个点在边界上，一个点不在边界上的情况
				if (v0.border != v1.border)  continue;

				vec3f p;
				// 计算 error 的同时, 计算坍缩后的 p 点的位置
				calculate_error(i0, i1, p);
				deleted0.resize(v0.tcount); // normals temporarily
				deleted1.resize(v1.tcount); // normals temporarily
				// 如果翻转, 则不删除
				if (flipped(p, i0, i1, v0, v1, deleted0)) continue;

				if (flipped(p, i1, i0, v1, v0, deleted1)) continue;

				if (t.attr == Attributes::TEXCOORD)
				{
					update_uvs(i0, v0, p, deleted0);
					update_uvs(i0, v1, p, deleted1);
				}

				// not flipped, so remove edge
				v0.p = p;
				v0.q = v1.q + v0.q;
				int tstart = refs.size();

				// 删除三角形
				update_triangles(i0, v0, deleted0, deleted_triangles);
				update_triangles(i0, v1, deleted1, deleted_triangles);

				int tcount = refs.size() - tstart;

				if (tcount <= v0.tcount)
				{
					// 节省内存的操作
					if (tcount)memcpy(&refs[v0.tstart], &refs[tstart], tcount * sizeof(Ref));
				}
				else
					// append
					v0.tstart = tstart;

				v0.tcount = tcount;
				break;
			}
			// done?
			if (triangle_count - deleted_triangles <= target_count)break;
		}
	}
	// 结束前删除网格
	compact_mesh();
} //simplify_mesh()

// Check if a triangle flips when this edge is removed
bool Simplify::flipped(vec3f p, int i0, int i1, Vertex& v0, Vertex& v1, std::vector<int>& deleted)
{

	loopk(0, v0.tcount)
	{
		Triangle& t = triangles[refs[v0.tstart + k].tid];
		if (t.deleted)continue;

		int s = refs[v0.tstart + k].tvertex;
		int id1 = t.v[(s + 1) % 3];
		int id2 = t.v[(s + 2) % 3];

		if (id1 == i1 || id2 == i1) // delete ?
		{

			deleted[k] = 1;
			continue;
		}
		vec3f d1 = vertices[id1].p - p; d1.normalize();
		vec3f d2 = vertices[id2].p - p; d2.normalize();
		if (fabs(d1.dot(d2)) > 0.999) return true; 
		vec3f n;
		n.cross(d1, d2);
		n.normalize();
		deleted[k] = 0;
		if (n.dot(t.n) < 0.2) return true; //共享一条边时法向量应该是相反的，现在相近所以发生了反转
	}
	return false;
}

// update_uvs

void Simplify::update_uvs(int i0, const Vertex& v, const vec3f& p, std::vector<int>& deleted)
{
	loopk(0, v.tcount)
	{
		Ref& r = refs[v.tstart + k];
		Triangle& t = triangles[r.tid];
		if (t.deleted)continue;
		if (deleted[k])continue;
		vec3f p1 = vertices[t.v[0]].p;
		vec3f p2 = vertices[t.v[1]].p;
		vec3f p3 = vertices[t.v[2]].p;
		t.uvs[r.tvertex] = interpolate(p, p1, p2, p3, t.uvs);
	}
}

// 更新三角形的连接关系，以及边的 error

void Simplify::update_triangles(int i0, Vertex& v, std::vector<int>& deleted, int& deleted_triangles)
{
	vec3f p;
	loopk(0, v.tcount)
	{
		Ref& r = refs[v.tstart + k];
		Triangle& t = triangles[r.tid];
		if (t.deleted)continue;
		if (deleted[k])
		{
			t.deleted = 1;
			deleted_triangles++;
			continue;
		}
		t.v[r.tvertex] = i0;
		t.dirty = 1;
		t.err[0] = calculate_error(t.v[0], t.v[1], p);
		t.err[1] = calculate_error(t.v[1], t.v[2], p);
		t.err[2] = calculate_error(t.v[2], t.v[0], p);
		t.err[3] = std::min(t.err[0], std::min(t.err[1], t.err[2]));
		refs.push_back(r);
	}
}

// compact triangles, compute edge error and build reference list

void Simplify::update_mesh(int iteration)
{
	if (iteration > 0) // compact triangles
	{
		int dst = 0;
		loopi(0, triangles.size())
			if (!triangles[i].deleted)
			{
				triangles[dst++] = triangles[i];
			}
		triangles.resize(dst);
	}
	//

	// Init Reference ID list
	loopi(0, vertices.size())
	{
		vertices[i].tstart = 0;
		vertices[i].tcount = 0;
	}
	loopi(0, triangles.size())
	{
		Triangle& t = triangles[i];
		loopj(0, 3) vertices[t.v[j]].tcount++;
	}
	int tstart = 0;
	loopi(0, vertices.size())
	{
		Vertex& v = vertices[i];
		v.tstart = tstart;
		tstart += v.tcount;
		v.tcount = 0;
	}

	// Write References
	refs.resize(triangles.size() * 3);
	loopi(0, triangles.size())
	{
		Triangle& t = triangles[i];
		loopj(0, 3)
		{
			Vertex& v = vertices[t.v[j]];
			refs[v.tstart + v.tcount].tid = i;
			refs[v.tstart + v.tcount].tvertex = j;
			v.tcount++;
		}
	}

	// Init Quadrics by Plane & Edge Errors
	//
	// required at the beginning ( iteration == 0 )
	// recomputing during the simplification is not required,
	// but mostly improves the result for closed meshes
	//
	if (iteration == 0)
	{
		// Identify boundary : vertices[].border=0,1

		std::vector<int> vcount, vids;

		loopi(0, vertices.size())
			vertices[i].border = 0;

		loopi(0, vertices.size())
		{
			Vertex& v = vertices[i];
			vcount.clear();
			vids.clear();
			loopj(0, v.tcount)
			{
				int k = refs[v.tstart + j].tid;
				Triangle& t = triangles[k];
				loopk(0, 3)
				{
					int ofs = 0, id = t.v[k];
					while (ofs < vcount.size())
					{
						if (vids[ofs] == id)break;
						ofs++;
					}
					if (ofs == vcount.size())
					{
						vcount.push_back(1);
						vids.push_back(id);
					}
					else
						vcount[ofs]++;
				}
			}
			loopj(0, vcount.size()) if (vcount[j] == 1)
				vertices[vids[j]].border = 1;
		}
		//initialize errors
		loopi(0, vertices.size())
			vertices[i].q = SymetricMatrix(0.0);

		loopi(0, triangles.size())
		{
			Triangle& t = triangles[i];
			vec3f n, p[3];
			loopj(0, 3) p[j] = vertices[t.v[j]].p;
			n.cross(p[1] - p[0], p[2] - p[0]);
			n.normalize();
			t.n = n;
			loopj(0, 3) vertices[t.v[j]].q =
				vertices[t.v[j]].q + SymetricMatrix(n.x, n.y, n.z, -n.dot(p[0]));
		}
		loopi(0, triangles.size())
		{
			// Calc Edge Error
			Triangle& t = triangles[i]; vec3f p;
			loopj(0, 3) t.err[j] = calculate_error(t.v[j], t.v[(j + 1) % 3], p);
			t.err[3] = std::min(t.err[0], std::min(t.err[1], t.err[2]));
		}
	}
}

// Finally compact mesh before exiting
void Simplify::compact_mesh()
{
	int dst = 0;
	loopi(0, vertices.size())
	{
		vertices[i].tcount = 0;
	}
	loopi(0, triangles.size())
		if (!triangles[i].deleted)
		{
			Triangle& t = triangles[i];
			triangles[dst++] = t;
			loopj(0, 3)vertices[t.v[j]].tcount = 1;
		}
	triangles.resize(dst);
	dst = 0;
	loopi(0, vertices.size())
		if (vertices[i].tcount)
		{
			vertices[i].tstart = dst;
			vertices[dst].p = vertices[i].p;
			dst++;
		}
	loopi(0, triangles.size())
	{
		Triangle& t = triangles[i];
		loopj(0, 3)t.v[j] = vertices[t.v[j]].tstart;
	}
	vertices.resize(dst);
}

// Error between vertex and Quadric
double Simplify::vertex_error(SymetricMatrix q, double x, double y, double z)
{
	return   q[0] * x * x + 2 * q[1] * x * y + 2 * q[2] * x * z + 2 * q[3] * x + q[4] * y * y
		+ 2 * q[5] * y * z + 2 * q[6] * y + q[7] * z * z + 2 * q[8] * z + q[9];
}

// Error for one edge
double Simplify::calculate_error(int id_v1, int id_v2, vec3f& p_result)
{
	// compute interpolated vertex
	SymetricMatrix q = vertices[id_v1].q + vertices[id_v2].q;
	bool   border = vertices[id_v1].border & vertices[id_v2].border;
	double error = 0;
	double det = q.det(0, 1, 2, 1, 4, 5, 2, 5, 7); // 计算行列式
	if (det != 0 && !border)
	{

		// q_delta is invertible
		p_result.x = -1 / det * (q.det(1, 2, 3, 4, 5, 6, 5, 7, 8));	// vx = A41/det(q_delta)
		p_result.y = 1 / det * (q.det(0, 2, 3, 1, 5, 6, 2, 7, 8));	// vy = A42/det(q_delta)
		p_result.z = -1 / det * (q.det(0, 1, 3, 1, 4, 6, 2, 5, 8));	// vz = A43/det(q_delta)

		error = vertex_error(q, p_result.x, p_result.y, p_result.z);
	}
	else
	{
		// det = 0 -> try to find best result
		vec3f p1 = vertices[id_v1].p;
		vec3f p2 = vertices[id_v2].p;
		vec3f p3 = (p1 + p2) / 2;
		double error1 = vertex_error(q, p1.x, p1.y, p1.z);
		double error2 = vertex_error(q, p2.x, p2.y, p2.z);
		double error3 = vertex_error(q, p3.x, p3.y, p3.z);
		error = std::min(error1, std::min(error2, error3));
		if (error1 == error) p_result = p1;
		if (error2 == error) p_result = p2;
		if (error3 == error) p_result = p3;
	}
	return error;
}


void Simplify::load_triangle(const std::vector<double>& in_vertices, const std::vector<unsigned int>& in_indices, const std::vector<float>& in_scalars)
{
	vertices.clear();
	triangles.clear();

	for (size_t i = 0; i < in_vertices.size(); i += 3)
	{
		Vertex v;
		v.p.x = in_vertices[i + 0];
		v.p.y = in_vertices[i + 1];
		v.p.z = in_vertices[i + 2];
		vertices.emplace_back(v);
	}
	int material = -1;

	for (size_t i = 0; i < in_indices.size(); i += 3)
	{
		Triangle t;
		for (int j = 0; j < 3; j++)
		{
			t.v[j] = in_indices[i + j];
			t.uvs[j].x = in_scalars[in_indices[i + j]];    // 利用 uvs[i].x 对标量进行插值
			t.uvs[j].y = 0;
			t.uvs[j].z = 0;
		}
		t.attr = Attributes::TEXCOORD;

		t.material = material;
		triangles.emplace_back(t);
	}
}

SimplifyObject Simplify::write_triangle()
{
	std::vector<double> out_vertices;
	std::vector<unsigned int> out_indices;
	std::vector<float> out_scalars;
	for (size_t i = 0; i < vertices.size(); i++)
	{
		out_vertices.emplace_back(vertices[i].p.x);
		out_vertices.emplace_back(vertices[i].p.y);
		out_vertices.emplace_back(vertices[i].p.z);
	}
	out_scalars.resize(out_vertices.size());
	for (auto t : triangles)
	{
		out_scalars[t.v[0]] = t.uvs[0].x;
		out_scalars[t.v[1]] = t.uvs[1].x;
		out_scalars[t.v[2]] = t.uvs[2].x;
	}
	for (auto t : triangles)
	{
		unsigned int v0 = t.v[0];
		unsigned int v1 = t.v[1];
		unsigned int v2 = t.v[2];
		out_indices.emplace_back(v0);
		out_indices.emplace_back(v1);
		out_indices.emplace_back(v2);
	}
	return { out_vertices, out_indices, out_scalars };
}

/**
 * @brif 简化三角形调用函数
 * @return SimplifyObject, 结构体, 包含简化后的顶点、索引和标量向量
 * @param vertices, 待简化顶点
 * @param indices, 待简化索引
 * @param scalars, 待简化标量
 * @param reduceFraction, 三角形数量减少率, 默认为 0.5 (减半)
 * @param agressiveness, 阈值参数, 取值为 5-8 时较好
**/
SimplifyObject Simplify::simplify(const std::vector<double>& vertices, const std::vector<unsigned int>& indices, const std::vector<float>& scalars,
	double reduceFraction, double agressiveness)
{
	load_triangle(vertices, indices, scalars);
	if ((Simplify::triangles.size() < 3) || (Simplify::vertices.size() < 3))
		throw std::runtime_error("can't make triangle");
	if (reduceFraction <= 0.0 || reduceFraction > 1.0)
	{
		throw std::runtime_error("Ratio must be BETWEEN zero and one.");
	}
	int target_count = round((float)Simplify::triangles.size() * reduceFraction);
	if (target_count < 4)
	{
		throw std::runtime_error("Object will not survive such extreme decimation");
	}
	clock_t start = clock();
	printf("Input: %zu vertices, %zu triangles (target %d)\n", Simplify::vertices.size(), Simplify::triangles.size(), target_count);
	int startSize = triangles.size();
	simplify_mesh(target_count, agressiveness, true);
	if (triangles.size() >= startSize) {
		throw std::runtime_error("Unable to reduce mesh.\n");
	}
	SimplifyObject obj;
	obj = write_triangle();
	printf("Output: %zu vertices, %zu triangles (%f reduction; %.4f sec)\n", Simplify::vertices.size(), Simplify::triangles.size()
		, (float)Simplify::triangles.size() / (float)startSize, ((float)(clock() - start)) / CLOCKS_PER_SEC);
	return obj;
}
SimplifyObject Simplify::simplify(const std::vector<double>& vertices, const std::vector<unsigned int>& indices,
	double reduceFraction, double agressiveness)
{
	std::vector<float> temp(vertices.size() / 3, 0);
	return simplify(vertices, indices, temp, reduceFraction, agressiveness);
}