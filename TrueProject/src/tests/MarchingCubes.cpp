#include "MarchingCubes.h"
#include <cmath>


namespace test
{
	MarchingCubes::MarchingCubes()
		: m_Proj(glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f)), m_View(camera.GetViewMatrix()), m_Ai(glm::vec3(0.992f, 0.957f, 0.863f))
	{
		//Set the light for the scene
		m_PointLight.m_Pos = camera.Position;
		m_PointLight.m_Id = glm::vec3(0.5, 0.5, 0.5);

		/*Need to populate the vertex buffer with the appropriate vertices*/

		//Create Grid composed of grid cells
		m_Grid = std::make_unique<Grid>(n);
		//Set vertex positions
		m_Positions = m_Grid->getTriangleVertices();

		//Activate buffer blending
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		//OpenGL setup
		m_Shader = std::make_unique<Shader>("res/shaders/MarchingCubesGourad.shader");
		m_VAO = std::make_unique<VertexArray>();
		//m_IBO = std::make_unique<IndexBuffer>(indices, 6); Not using this for now
		//m_Texture = std::make_unique<Texture>("res/textures/ChernoLogo.png"); Not using this for now
		m_VB = std::make_unique<VertexBuffer>(m_Positions, 3 * m_Grid->getSizeOfTriangleVertices() * sizeof(float), m_Grid->getSizeOfTriangleVertices());
		VertexBufferLayout layout;
		layout.push<float>(3u); //First attrib
		layout.push<float>(3u);
		//layout.push<float>(2u);
		m_VAO->AddBuffer(*m_VB, layout);

		//Set uniforms
		/*std::cout << "m_Ai.x: " << m_Ai.x << ", m_Ai.y: " << m_Ai.y << ", m_Ai.z: " << m_Ai.z << std::endl;
		std::cout << "m_PointLight.m_Id.x: " << m_PointLight.m_Id.x << ", m_PointLight.m_Id.y: " << m_PointLight.m_Id.y << ", m_PointLight.m_Id.z: " << m_PointLight.m_Id.z << std::endl;
		std::cout << "m_PointLight.m_Pos.x: " << m_PointLight.m_Pos.x << ", m_PointLight.m_Pos.y: " << m_PointLight.m_Pos.y << ", m_PointLight.m_Pos.z: " << m_PointLight.m_Pos.z << std::endl;

		std::cout << "ka: " << m_Grid->function->getKa() << std::endl;
		std::cout << "kd: " << m_Grid->function->getKd() << std::endl;

		std::cout << "m_Grid->function->getAc().x: " << m_Grid->function->getAc().x << ", m_Grid->function->getAc().y: " << m_Grid->function->getAc().y << ", m_Grid->function->getAc().z: " << m_Grid->function->getAc().z << std::endl;
		std::cout << "m_Grid->function->getDc().x: " << m_Grid->function->getDc().x << ", m_Grid->function->getDc().y: " << m_Grid->function->getDc().y << ", m_Grid->function->getDc().z: " << m_Grid->function->getDc().z << std::endl;*/
		m_Shader->Bind();
		m_Shader->setUniform3f("ai", m_Ai.x, m_Ai.y, m_Ai.z);
		m_Shader->setUniform3f("id", m_PointLight.m_Id.x, m_PointLight.m_Id.y, m_PointLight.m_Id.z);
		m_Shader->setUniform3f("lightPos", m_PointLight.m_Pos.x, m_PointLight.m_Pos.y, m_PointLight.m_Pos.z);
		m_Shader->setUniform3f("cameraPos", camera.Position.x, camera.Position.y, camera.Position.z);
		m_Shader->setUniform1f("ka", m_Grid->function->getKa());
		m_Shader->setUniform1f("kd", m_Grid->function->getKd());
		m_Shader->setUniform3f("ac", m_Grid->function->getAc().x, m_Grid->function->getAc().y, m_Grid->function->getAc().z);
		m_Shader->setUniform3f("dc", m_Grid->function->getDc().x, m_Grid->function->getDc().y, m_Grid->function->getDc().z);
	}
	
	MarchingCubes::~MarchingCubes()
	{
		delete m_Positions;
	}
	void MarchingCubes::OnUpdate(float deltaTime)
	{
		//Uniforms update
		//Update the transformation matrices
		m_Proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		m_View = camera.GetViewMatrix();

		//Update the lighting position
		m_PointLight.m_Pos = camera.Position;
		m_Shader->Bind();
		m_Shader->setUniform3f("lightPos", m_PointLight.m_Pos.x, m_PointLight.m_Pos.y, m_PointLight.m_Pos.z);

		//Update the camera position
		m_Shader->setUniform3f("cameraPos", camera.Position.x, camera.Position.y, camera.Position.z);
		//Sample new points based on the updated isovalue (all updates relating to the grid are done inside m_Grid->sample())
		m_Grid->new_sample();

		//Update the positions
		delete m_Positions;

		m_Positions = m_Grid->getTriangleVertices();

		//for (int i = 0; i < 3 * m_Grid->getSizeOfTriangleVertices(); i++)
		//{
		//	std::cout << "vertex " << i <<  ": " << m_Positions[i] << std::endl;
		//}

		//Refill the buffer
		m_VB->refill(m_Positions, 3 * m_Grid->getSizeOfTriangleVertices() * sizeof(float), m_Grid->getSizeOfTriangleVertices());
	}

	void MarchingCubes::OnRender()
	{

		Renderer renderer;

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->setUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_VB, *m_Shader);
		}
	}

	void MarchingCubes::OnImGuiRender()
	{
		ImGui::SliderFloat("Isovalue", &m_Grid->m_IsoValue, 0.0f, 30.0f);
		ImGui::SliderFloat("Grid starting position scale", &m_Grid->m_StartScale, 1.0f, 15.0f);
		ImGui::SliderFloat("Grid edge size scale", &m_Grid->m_EdgeScale, 1.0, 15.0f);
		ImGui::SliderInt("Grid edge resolution scale", &n, 1, 25);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	Grid::Grid(int n_val) : m_IsoValue(0.8f), m_Size(n_val), m_Start(glm::vec3(-1.0, -1.0, 1.0)), m_StartScale(1.0f), m_EdgeScale(1.0f)
	{

		m_Width = 2;
		m_Height = 2;
		m_Depth = 2;

		//Set the basis vectors
		m_BasisU = glm::vec3(m_Width, 0.0f, 0.0f) / (float)m_Size;
		m_BasisV = glm::vec3(0.0f, m_Height, 0.0f) / (float)m_Size;
		m_BasisL = glm::vec3(0.0f, 0.0f, -m_Depth) / (float)m_Size;

		//Set the function eval strategy
		if (function_name == "Sphere")
		{
			function = std::make_unique<Sphere>(0.6f, 0.6f, glm::vec3(0, 0, 0.5), glm::vec3(0.0f, 0.4f, 0.7f));
		}
		else if (function_name == "Taurus")
		{
			function = std::make_unique<Taurus>(0.6f, 0.6f, glm::vec3(0, 0, 0.5), glm::vec3(0.0f, 0.4f, 0.7f));
		}
		else if (function_name == "Genus")
		{
			function = std::make_unique<Genus>(0.6f, 0.6f, glm::vec3(0, 0, 0.5), glm::vec3(0.0f, 0.4f, 0.7f));
		}
		else if (function_name == "CassiniOval")
		{
			function = std::make_unique<CassiniOval>(0.6f, 0.6f, glm::vec3(0, 0, 0.5), glm::vec3(0.0f, 0.4f, 0.7f));
		}
		else if (function_name == "TauriIntersect")
		{
			function = std::make_unique<TauriIntersect>(0.6f, 0.6f, glm::vec3(0, 0, 0.5), glm::vec3(0.0f, 0.4f, 0.7f));
		}
		else if (function_name == "SurfaceOfRevolution")
		{
			function = std::make_unique<SurfaceOfRevolution>(0.6f, 0.6f, glm::vec3(0, 0, 0.5), glm::vec3(0.0f, 0.4f, 0.7f));
		}
		else if (function_name == "CustomFunc")
		{
			function = std::make_unique<CustomFunc>(0.6f, 0.6f, glm::vec3(0, 0, 0.5), glm::vec3(0.0f, 0.4f, 0.7f));
		}

		m_TriangleVertices = std::vector<glm::vec3>();

		m_GridCells.resize(m_Size);

		//Create the grid cells
		for (float x = 0; x < m_Size; x++)
		{
			m_GridCells[x].resize(m_Size);

			for (float y = 0; y < m_Size; y++)
			{
				m_GridCells[x][y].resize(m_Size);
				for (float z = 0; z < m_Size; z++)
				{
					m_GridCells[x][y][z] = std::make_unique<GridCell>();

					glm::vec3 new_start = m_Start + z * m_BasisL + y * m_BasisV + x * m_BasisU;

					//Set the vertices and values for this GridCell
					m_GridCells[x][y][z]->m_Vertices[0] = new_start;
					m_GridCells[x][y][z]->m_Values[0] = function->eval(m_GridCells[x][y][z]->m_Vertices[0]);
					m_GridCells[x][y][z]->m_Vertices[1] = new_start + m_BasisL;
					m_GridCells[x][y][z]->m_Values[1] = function->eval(m_GridCells[x][y][z]->m_Vertices[1]);
					m_GridCells[x][y][z]->m_Vertices[2] = new_start + m_BasisL +  m_BasisU;
					m_GridCells[x][y][z]->m_Values[2] = function->eval(m_GridCells[x][y][z]->m_Vertices[2]);
					m_GridCells[x][y][z]->m_Vertices[3] = new_start + m_BasisU;
					m_GridCells[x][y][z]->m_Values[3] = function->eval(m_GridCells[x][y][z]->m_Vertices[3]);
					m_GridCells[x][y][z]->m_Vertices[4] = new_start + m_BasisV;
					m_GridCells[x][y][z]->m_Values[4] = function->eval(m_GridCells[x][y][z]->m_Vertices[4]);
					m_GridCells[x][y][z]->m_Vertices[5] = new_start + m_BasisV + m_BasisL;
					m_GridCells[x][y][z]->m_Values[5] = function->eval(m_GridCells[x][y][z]->m_Vertices[5]);
					m_GridCells[x][y][z]->m_Vertices[6] = new_start + m_BasisV + m_BasisL + m_BasisU;
					m_GridCells[x][y][z]->m_Values[6] = function->eval(m_GridCells[x][y][z]->m_Vertices[6]);
					m_GridCells[x][y][z]->m_Vertices[7] = new_start + m_BasisV + m_BasisU;
					m_GridCells[x][y][z]->m_Values[7] = function->eval(m_GridCells[x][y][z]->m_Vertices[7]);
					//Get normal at each vertex
					getNormals(*m_GridCells[x][y][z]);

					//Get vertices from grid cell
					getVertices(*m_GridCells[x][y][z], m_TriangleVertices);
					
				}
			}
		}
	}

	Grid::~Grid()
	{
	}

	float* Grid::getTriangleVertices()
	{
		float* vertices = new float[3 * m_TriangleVertices.size()];
		int i{ 0 };
		//Triangle Vertices takes into account the normals as well
		for (auto& vertex : m_TriangleVertices)
		{
			*(vertices + i++) = vertex.x;
			*(vertices + i++) = vertex.y;
			*(vertices + i++) = vertex.z;
			//std::cout << "normal: x(" << m_TriangleNormals[j].x << "), y(" << m_TriangleNormals[j].y << "), z(" << m_TriangleNormals[j].z << ")" << std::endl;
		}

		/*for (int j = 0; j < 3 * m_TriangleVertices.size(); j++)
		{
			std::cout << "j = " <<  j << " (" << *(positions + j) << ")" << std::endl;
		}

		std::cout << "size of positions: " << 3 * m_TriangleVertices.size() << std::endl;*/

		return vertices;
	}

	void Grid::new_sample()
	{
		//Delete old values inside m_TriangleVertices
		m_TriangleVertices.clear();

		//Udpate variables based on scale
		m_Start = glm::vec3(-1.0,-1.0,1.0) * m_StartScale;

		m_Width = 2 * m_EdgeScale;
		m_Height = 2 * m_EdgeScale;
		m_Depth = 2 * m_EdgeScale;

		m_Size = n;

		//Set the basis vectors
		m_BasisU = glm::vec3(m_Width, 0.0f, 0.0f) / (float)m_Size;
		m_BasisV = glm::vec3(0.0f, m_Height, 0.0f) / (float)m_Size;
		m_BasisL = glm::vec3(0.0f, 0.0f, -m_Depth) / (float)m_Size;


		m_GridCells.resize(m_Size);
		for (float x = 0; x < m_Size; x++)
		{
			m_GridCells[x].resize(m_Size);
			for (float y = 0; y < m_Size; y++)
			{
				m_GridCells[x][y].resize(m_Size);
				for (float z = 0; z < m_Size; z++)
				{
					m_GridCells[x][y][z] = std::make_unique<GridCell>();
					glm::vec3 new_start = m_Start + z * m_BasisL + y * m_BasisV + x * m_BasisU;
					m_GridCells[x][y][z]->m_Vertices[0] = new_start;
					m_GridCells[x][y][z]->m_Values[0] = function->eval(m_GridCells[x][y][z]->m_Vertices[0]);
					m_GridCells[x][y][z]->m_Vertices[1] = new_start + m_BasisL;
					m_GridCells[x][y][z]->m_Values[1] = function->eval(m_GridCells[x][y][z]->m_Vertices[1]);
					m_GridCells[x][y][z]->m_Vertices[2] = new_start + m_BasisL + m_BasisU;
					m_GridCells[x][y][z]->m_Values[2] = function->eval(m_GridCells[x][y][z]->m_Vertices[2]);
					m_GridCells[x][y][z]->m_Vertices[3] = new_start + m_BasisU;
					m_GridCells[x][y][z]->m_Values[3] = function->eval(m_GridCells[x][y][z]->m_Vertices[3]);
					m_GridCells[x][y][z]->m_Vertices[4] = new_start + m_BasisV;
					m_GridCells[x][y][z]->m_Values[4] = function->eval(m_GridCells[x][y][z]->m_Vertices[4]);
					m_GridCells[x][y][z]->m_Vertices[5] = new_start + m_BasisV + m_BasisL;
					m_GridCells[x][y][z]->m_Values[5] = function->eval(m_GridCells[x][y][z]->m_Vertices[5]);
					m_GridCells[x][y][z]->m_Vertices[6] = new_start + m_BasisV + m_BasisL + m_BasisU;
					m_GridCells[x][y][z]->m_Values[6] = function->eval(m_GridCells[x][y][z]->m_Vertices[6]);
					m_GridCells[x][y][z]->m_Vertices[7] = new_start + m_BasisV + m_BasisU;
					m_GridCells[x][y][z]->m_Values[7] = function->eval(m_GridCells[x][y][z]->m_Vertices[7]);
					//Get normal at each vertex
					getNormals(*m_GridCells[x][y][z]);

					//Get new vertices from grid cells
					getVertices(*m_GridCells[x][y][z], m_TriangleVertices);

				}
			}
		}
	}

	void Grid::getVertices(const GridCell& gridCell, std::vector<glm::vec3>& allVertices)
	{
		//Get index into edge table
		int cubeIndex = 0;

		for (int i = 0; i < 8; i++)
		{
			if (gridCell.m_Values[i] < m_IsoValue)
			{
				cubeIndex += 1 << i;
			}
		}

		glm::vec3 vertexList[12];
		glm::vec3 normalList[12];

		if (edgeTable[cubeIndex] & 1)
			vertexList[0] =
			interpolateVertex(m_IsoValue, gridCell.m_Vertices[0], gridCell.m_Vertices[1], gridCell.m_Values[0], gridCell.m_Values[1]);
			normalList[0] =
			interpolateNormal(m_IsoValue, gridCell.m_Normals[0], gridCell.m_Normals[1], gridCell.m_Values[0], gridCell.m_Values[1]);
		if (edgeTable[cubeIndex] & 2)
			vertexList[1] =
			interpolateVertex(m_IsoValue, gridCell.m_Vertices[1], gridCell.m_Vertices[2], gridCell.m_Values[1], gridCell.m_Values[2]);
			normalList[1] =
			interpolateNormal(m_IsoValue, gridCell.m_Normals[1], gridCell.m_Normals[2], gridCell.m_Values[1], gridCell.m_Values[2]);
		if (edgeTable[cubeIndex] & 4)
			vertexList[2] =
			interpolateVertex(m_IsoValue, gridCell.m_Vertices[2], gridCell.m_Vertices[3], gridCell.m_Values[2], gridCell.m_Values[3]);
			normalList[2] =
			interpolateNormal(m_IsoValue, gridCell.m_Normals[2], gridCell.m_Normals[3], gridCell.m_Values[2], gridCell.m_Values[3]);
		if (edgeTable[cubeIndex] & 8)
			vertexList[3] =
			interpolateVertex(m_IsoValue, gridCell.m_Vertices[3], gridCell.m_Vertices[0], gridCell.m_Values[3], gridCell.m_Values[0]);
			normalList[3] =
			interpolateNormal(m_IsoValue, gridCell.m_Normals[3], gridCell.m_Normals[0], gridCell.m_Values[3], gridCell.m_Values[0]);
		if (edgeTable[cubeIndex] & 16)
			vertexList[4] =
			interpolateVertex(m_IsoValue, gridCell.m_Vertices[4], gridCell.m_Vertices[5], gridCell.m_Values[4], gridCell.m_Values[5]);
			normalList[4] =
			interpolateNormal(m_IsoValue, gridCell.m_Normals[4], gridCell.m_Normals[5], gridCell.m_Values[4], gridCell.m_Values[5]);
		if (edgeTable[cubeIndex] & 32)
			vertexList[5] =
			interpolateVertex(m_IsoValue, gridCell.m_Vertices[5], gridCell.m_Vertices[6], gridCell.m_Values[5], gridCell.m_Values[6]);
			normalList[5] =
			interpolateNormal(m_IsoValue, gridCell.m_Normals[5], gridCell.m_Normals[6], gridCell.m_Values[5], gridCell.m_Values[6]);
		if (edgeTable[cubeIndex] & 64)
			vertexList[6] =
			interpolateVertex(m_IsoValue, gridCell.m_Vertices[6], gridCell.m_Vertices[7], gridCell.m_Values[6], gridCell.m_Values[7]);
			normalList[6] =
			interpolateNormal(m_IsoValue, gridCell.m_Normals[6], gridCell.m_Normals[7], gridCell.m_Values[6], gridCell.m_Values[7]);
		if (edgeTable[cubeIndex] & 128)
			vertexList[7] =
			interpolateVertex(m_IsoValue, gridCell.m_Vertices[7], gridCell.m_Vertices[4], gridCell.m_Values[7], gridCell.m_Values[4]);
			normalList[7] =
			interpolateNormal(m_IsoValue, gridCell.m_Normals[7], gridCell.m_Normals[4], gridCell.m_Values[7], gridCell.m_Values[4]);
		if (edgeTable[cubeIndex] & 256)
			vertexList[8] =
			interpolateVertex(m_IsoValue, gridCell.m_Vertices[0], gridCell.m_Vertices[4], gridCell.m_Values[0], gridCell.m_Values[4]);
			normalList[8] =
			interpolateNormal(m_IsoValue, gridCell.m_Normals[0], gridCell.m_Normals[4], gridCell.m_Values[0], gridCell.m_Values[4]);
		if (edgeTable[cubeIndex] & 512)
			vertexList[9] =
			interpolateVertex(m_IsoValue, gridCell.m_Vertices[1], gridCell.m_Vertices[5], gridCell.m_Values[1], gridCell.m_Values[5]);
			normalList[9] =
			interpolateNormal(m_IsoValue, gridCell.m_Normals[1], gridCell.m_Normals[5], gridCell.m_Values[1], gridCell.m_Values[5]);
		if (edgeTable[cubeIndex] & 1024)
			vertexList[10] =
			interpolateVertex(m_IsoValue, gridCell.m_Vertices[2], gridCell.m_Vertices[6], gridCell.m_Values[2], gridCell.m_Values[6]);
			normalList[10] =
			interpolateNormal(m_IsoValue, gridCell.m_Normals[2], gridCell.m_Normals[6], gridCell.m_Values[2], gridCell.m_Values[6]);
		if (edgeTable[cubeIndex] & 2048)
			vertexList[11] =
			interpolateVertex(m_IsoValue, gridCell.m_Vertices[3], gridCell.m_Vertices[7], gridCell.m_Values[3], gridCell.m_Values[7]);
			normalList[11] =
			interpolateNormal(m_IsoValue, gridCell.m_Normals[3], gridCell.m_Normals[7], gridCell.m_Values[3], gridCell.m_Values[7]);

		//Create triangles
		for (int i = 0; triTable[cubeIndex][i] != -1; i += 3) {
			allVertices.push_back(vertexList[triTable[cubeIndex][i]]);
			allVertices.push_back(normalList[triTable[cubeIndex][i]]);
			allVertices.push_back(vertexList[triTable[cubeIndex][i + 1]]);
			allVertices.push_back(normalList[triTable[cubeIndex][i + 1]]);
			allVertices.push_back(vertexList[triTable[cubeIndex][i + 2]]);
			allVertices.push_back(normalList[triTable[cubeIndex][i + 2]]);
		}
	}
	void Grid::getNormals(GridCell& gridCell)
	{
		for (int i = 0; i < 8; i++)
		{
			float grad_x = (function->eval(gridCell.m_Vertices[i] + m_BasisU) - function->eval(gridCell.m_Vertices[i] - m_BasisU)) / (glm::length(m_BasisU));
			float grad_y = (function->eval(gridCell.m_Vertices[i] + m_BasisV) - function->eval(gridCell.m_Vertices[i] - m_BasisV)) / (glm::length(m_BasisV));
			float grad_z = (function->eval(gridCell.m_Vertices[i] + m_BasisL) - function->eval(gridCell.m_Vertices[i] - m_BasisL)) / (glm::length(m_BasisL));
			gridCell.m_Normals[i] = glm::vec3(grad_x, grad_y, grad_z);
		}
	}
	glm::vec3 Grid::interpolateVertex(float m_IsoValue, glm::vec3 p1, glm::vec3 p2, float val_p1, float val_p2)
	{
		glm::vec3 p;
		float mu;

		if (std::abs(m_IsoValue - val_p1) < 0.00001)
			return(p1);
		if (std::abs(m_IsoValue - val_p2) < 0.00001)
			return(p2);
		if (std::abs(val_p1 - val_p2) < 0.00001)
			return(p1);

		mu = (m_IsoValue - val_p1) / (val_p2 - val_p1);

		p.x = p1.x + mu * (p2.x - p1.x);
		p.y = p1.y + mu * (p2.y - p1.y);
		p.z = p1.z + mu * (p2.z - p1.z);

		return p;
	}

	glm::vec3 Grid::interpolateNormal(float m_IsoValue, glm::vec3 n1, glm::vec3 n2, float val_p1, float val_p2)
	{
		glm::vec3 n;
		float mu;

		if (std::abs(m_IsoValue - val_p1) < 0.00001)
			return(n1);
		if (std::abs(m_IsoValue - val_p2) < 0.00001)
			return(n2);
		if (std::abs(val_p1 - val_p2) < 0.00001)
			return(n1);

		mu = (m_IsoValue - val_p1) / (val_p2 - val_p1);

		n.x = n1.x + mu * (n2.x - n1.x);
		n.y = n1.y + mu * (n2.y - n1.y);
		n.z = n1.z + mu * (n2.z - n1.z);

		return n;
	}
}
