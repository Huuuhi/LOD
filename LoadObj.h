#pragma once
#include <vector>
#include <string>
#include <algorithm>
void load_obj(std::string name, std::vector<double>& vertices, std::vector<unsigned int>& triangles)
{
	std::string filename = "E:/GooYu/工程中心/LOD_release/" + name;
	FILE* fn;

	if (fopen_s(&fn, filename.c_str(), "rb") != 0)
	{
		printf("File %s not found!\n", filename.c_str());
		return;
	}
	char line[1000];
	memset ( line,0,1000 );
	int vertex_cnt = 0;

	while(fgets( line, 1000, fn ) != NULL)
	{
		Vertex v;
		vec3f uv;


		if ( line[0] == 'v' && line[1] == 't' )
		{

		}
		else if (line[0] == 'v' && line[1] == 'n')
		{

		}
		else if ( line[0] == 'v' )
		{
			double x, y, z;
			if ( line[1] == ' ' )
			if(sscanf_s(line,"v %lf %lf %lf",
				&x,	&y,	&z)==3)
			{
				//vertices.push_back(x / 300.0f);
				//vertices.push_back(y / 300.0f);
				//vertices.push_back(z / 300.0f);
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
			}
		}
		int integers[9];
		if ( line[0] == 'f' )
		{
			Triangle t;
			bool tri_ok = false;
            bool has_uv = false;

			if(sscanf_s(line,"f %d %d %d",
				&integers[0],&integers[1],&integers[2])==3)
			{
				tri_ok = true;
			}
			else if(sscanf_s(line,"f %d// %d// %d//",
				&integers[0],&integers[1],&integers[2])==3)
			{
				tri_ok = true;
			}
			else if(sscanf_s(line,"f %d//%d %d//%d %d//%d",
				&integers[0],&integers[3],
				&integers[1],&integers[4],
				&integers[2],&integers[5])==6)
			{
				tri_ok = true;
			}
			else if(sscanf_s(line,"f %d/%d/%d %d/%d/%d %d/%d/%d",
				&integers[0],&integers[6],&integers[3],
				&integers[1],&integers[7],&integers[4],
				&integers[2],&integers[8],&integers[5])==9)
			{
				tri_ok = true;
				has_uv = true;
			}
			else if (sscanf_s(line, "f %d/%d %d/%d %d/%d",// Add Support for v/vt only meshes
				&integers[0], &integers[6],
				&integers[1], &integers[7],
				&integers[2], &integers[8]) == 6)
			{
				tri_ok = true;
				has_uv = true;
			}
			else
			{
				printf("unrecognized sequence\n");
				printf("%s\n",line);
				while(1);
			}
			if ( tri_ok )
			{
				triangles.push_back(integers[0] - 1);
				triangles.push_back(integers[1] - 1);
				triangles.push_back(integers[2] - 1);
			}
		}
	}


	fclose(fn);

} 

void load_obj(std::string name, std::vector<double>& vertices, std::vector<unsigned int>& triangles, std::vector<float>& scalars,
	double dx = 0, double dy = 0, double dz = 0, double xx = 1, double yy = 1, double zz = 1)
{
	std::vector<float> currentScalars;
	std::string filename = "E:/GooYu/工程中心/LOD_release/" + name;
	FILE* fn;
	unsigned int index = vertices.size() / 3;
	if (fopen_s(&fn, filename.c_str(), "rb") != 0)
	{
		printf("File %s not found!\n", filename.c_str());
		return;
	}
	char line[1000];
	memset(line, 0, 1000);
	int vertex_cnt = 0;

	while (fgets(line, 1000, fn) != NULL)
	{
		Vertex v;
		vec3f uv;


		if (line[0] == 'v' && line[1] == 't')
		{

		}
		else if (line[0] == 'v' && line[1] == 'n')
		{

		}
		else if (line[0] == 'v')
		{
			double x, y, z;
			if (line[1] == ' ')
			{
				if (sscanf_s(line, "v %lf %lf %lf",
					&x, &y, &z) == 3)
				{
					//vertices.push_back(x / 300.0f);
					//vertices.push_back(y / 300.0f);
					//vertices.push_back(z / 300.0f);
					currentScalars.push_back(x);
					vertices.push_back(x * xx + dx);
					vertices.push_back(y * yy + dy);
					vertices.push_back(z * zz + dz);
				}
			}
		}
		int integers[9];
		if (line[0] == 'f')
		{
			Triangle t;
			bool tri_ok = false;
			bool has_uv = false;

			if (sscanf_s(line, "f %d %d %d",
				&integers[0], &integers[1], &integers[2]) == 3)
			{
				tri_ok = true;
			}
			else if (sscanf_s(line, "f %d// %d// %d//",
				&integers[0], &integers[1], &integers[2]) == 3)
			{
				tri_ok = true;
			}
			else if (sscanf_s(line, "f %d//%d %d//%d %d//%d",
				&integers[0], &integers[3],
				&integers[1], &integers[4],
				&integers[2], &integers[5]) == 6)
			{
				tri_ok = true;
			}
			else if (sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
				&integers[0], &integers[6], &integers[3],
				&integers[1], &integers[7], &integers[4],
				&integers[2], &integers[8], &integers[5]) == 9)
			{
				tri_ok = true;
				has_uv = true;
			}
			else if (sscanf_s(line, "f %d/%d %d/%d %d/%d",// Add Support for v/vt only meshes
				&integers[0], &integers[6],
				&integers[1], &integers[7],
				&integers[2], &integers[8]) == 6)
			{
				tri_ok = true;
				has_uv = true;
			}
			else
			{
				printf("unrecognized sequence\n");
				printf("%s\n", line);
				while (1);
			}
			if (tri_ok)
			{
				triangles.push_back(integers[0] - 1 + index);
				triangles.push_back(integers[1] - 1 + index);
				triangles.push_back(integers[2] - 1 + index);
			}
		}
	}
	//auto minScalar = *std::min_element(currentScalars.begin(), currentScalars.end());
	//auto maxScalar = *std::max_element(currentScalars.begin(), currentScalars.end());
	float maxScalar = std::numeric_limits<float>::lowest();
	float minScalar = std::numeric_limits<float>::max();
	
	for (auto s : currentScalars)
	{
		minScalar = std::min(minScalar, s);
		maxScalar = std::max(maxScalar, s);
	}
	for (auto &s : currentScalars)
	{
		s = (s - minScalar) / (maxScalar - minScalar);
	}
	scalars.insert(scalars.end(), currentScalars.begin(), currentScalars.end());
	fclose(fn);

}