#include "MyOctant.h"

//Look at the comments

using namespace Simplex;

uint MyOctant::m_uMyOctantCount = 0;
uint MyOctant::m_uMaxLevel = 3;
uint MyOctant::m_uIdealEntityCount = 5;

	void MyOctant::Init(void)
	{
		//Set the default values for these variables
		m_uChildren = 0;
		m_fSize = 0;
		m_uID = m_uMyOctantCount;
		m_uLevel = 0;
		m_v3Center = vector3(0.0f);
		m_v3Min = vector3(0.0f);
		m_v3Max = vector3(0.0f);

		m_pMeshMngr = MeshManager::GetInstance();
		m_pEntityMngr = MyEntityManager::GetInstance();

		m_pRoot = nullptr;
		m_pParent = nullptr;

		for (uint i = 0; i < 8; i++) {
			m_pChild[i] = nullptr;
		}
	}

	void MyOctant::Swap(MyOctant& other)
	{
		//call the swap method on literally every variable
		std::swap(m_uChildren, other.m_uChildren);

		std::swap(m_fSize, other.m_fSize);
		std::swap(m_uID, other.m_uID);
		std::swap(m_pRoot, other.m_pRoot);
		std::swap(m_lChild, other.m_lChild);

		std::swap(m_v3Center, other.m_v3Center);
		std::swap(m_v3Min, other.m_v3Min);
		std::swap(m_v3Max, other.m_v3Max);

		m_pMeshMngr = MeshManager::GetInstance();
		m_pEntityMngr = MyEntityManager::GetInstance();

		std::swap(m_uLevel, other.m_uLevel);
		std::swap(m_pParent, other.m_pParent);

		for (uint i = 0; i < 8; i++) {
			std::swap(m_pChild[i], other.m_pChild[i]);
		}

	}

	MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
	{
		//Initialize the variables for this new object
		Init();
		//set the max level and ideal count
		m_uMyOctantCount = 0;
		m_uMaxLevel = a_nMaxLevel;
		m_uIdealEntityCount = a_nIdealEntityCount;
		m_uID = m_uMyOctantCount;

		//kill the children array
		m_pRoot = this;
		m_lChild.clear();

		std::vector<vector3> lMinMax;

		uint nObjects = m_pEntityMngr->GetEntityCount();
		for (uint i = 0; i < nObjects; i++)
		{
			MyEntity* pEntity = m_pEntityMngr->GetEntity(i);
			MyRigidBody* pRigidBody = pEntity->GetRigidBody();
			lMinMax.push_back(pRigidBody->GetMinGlobal());
			lMinMax.push_back(pRigidBody->GetMaxGlobal());
		}

		MyRigidBody* pRigidBody = new MyRigidBody(lMinMax);


		vector3 vHalfWidth = pRigidBody->GetHalfWidth();
		float fMax = vHalfWidth.x;
		for (int i = 0; i < 3; i++)
		{
			if (fMax < vHalfWidth[i]) {
				fMax = vHalfWidth[i];
			}
		}

		//get the center point
		vector3 v3Center = pRigidBody->GetCenterLocal();
		lMinMax.clear();
		SafeDelete(pRigidBody);

		m_fSize = fMax * 2.0f;
		m_v3Center = v3Center;
		m_v3Min = m_v3Center - (vector3(fMax));
		m_v3Max = m_v3Center + (vector3(fMax));
	
		m_uMyOctantCount++;

		//Build the octree
		ConstructTree(m_uMaxLevel);
	}

	MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
	{
		//initialize the variables
		Init();

		//center the cube at the given vector
		m_v3Center = a_v3Center;
		m_fSize = a_fSize;

		m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
		m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);

		m_uMyOctantCount++;

	}

	MyOctant::MyOctant(MyOctant const& other)
	{
		//standard copy assignment
		m_uChildren = other.m_uChildren;
		m_v3Center = other.m_v3Center;
		m_v3Min = other.m_v3Min;
		m_v3Max = other.m_v3Max;

		m_fSize = other.m_fSize;
		m_uID = other.m_uID;
		m_uLevel = other.m_uLevel;
		m_pParent = other.m_pParent;

		m_pRoot = other.m_pRoot;
		m_lChild = other.m_lChild;

		m_pMeshMngr = MeshManager::GetInstance();
		m_pEntityMngr = MyEntityManager::GetInstance();


		for (uint i = 0; i < 8; i++) {
			m_pChild[i] = other.m_pChild[i];
		}

	}

	MyOctant& Simplex::MyOctant::operator=(MyOctant const& other)
	{
		//Standard assignment constructor
		if (this != &other) {
			Release();
			Init();
			MyOctant temp(other);
			Swap(temp);
		}
		return *this;
	}

	MyOctant::~MyOctant(void){Release();}

	float MyOctant::GetSize(void) { return m_fSize; }
	vector3 MyOctant::GetCenterGlobal(void) { return m_v3Center; }
	vector3 MyOctant::GetMinGlobal(void) { return m_v3Min; }
	vector3 MyOctant::GetMaxGlobal(void) { return m_v3Max; }
	MyOctant* MyOctant::GetChild(uint a_nChild)
	{
		//if the id exceeds the max number of children, return nothing
		if (a_nChild > 7)
		{
			return nullptr;
		}
		//return the child
		return m_pChild[a_nChild];
	}
	MyOctant* MyOctant::GetParent(void){return m_pParent;}
	bool MyOctant::IsLeaf(void){return m_uChildren==0;}

	bool MyOctant::IsColliding(uint a_uRBIndex)
	{
		//check if you're checking something out of bounds
		uint nObjectCount = m_pEntityMngr->GetEntityCount();

		if (a_uRBIndex >= nObjectCount)
		{
			return false;
		}

		//simple aabb collision checking
		MyEntity* entityPointer = m_pEntityMngr->GetEntity(a_uRBIndex);
		MyRigidBody* rigBody = entityPointer->GetRigidBody();
		vector3 v3MinO = rigBody->GetMinGlobal();
		vector3 v3MaxO = rigBody->GetMaxGlobal();

		if (m_v3Max.x < v3MinO.x) {
			return false;
		}
		if (m_v3Min.x > v3MaxO.x) {
			return false;
		}

		if (m_v3Max.y < v3MinO.y) {
			return false;
		}
		if (m_v3Min.y > v3MaxO.y) {
			return false;
		}

		if (m_v3Max.z < v3MinO.z) {
			return false;
		}
		if (m_v3Min.z > v3MaxO.z) {
			return false;
		}

		//no collision occured
		return true;

	}

	void MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
	{
		//display the curent or all it's children
		if (m_uID == a_nIndex)
		{
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
			return;
		}
		for (uint nIndex = 0; nIndex < m_uChildren; nIndex++)
		{
			m_pChild[nIndex]->Display(a_v3Color);
		}
	}

	void MyOctant::Display(vector3 a_v3Color)
	{
		//display the children and the parent
		for (uint nIndex = 0; nIndex < m_uChildren; nIndex++)
		{
			m_pChild[nIndex]->Display(a_v3Color);
		}
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
	}

	void MyOctant::DisplayLeafs(vector3 a_v3Color)
	{
		//display the children then the parent
		uint nLeafs = m_lChild.size();
		for (uint i = 0; i < nLeafs; i++)
		{
			m_lChild[i]->DisplayLeafs(a_v3Color);
		}
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center)*glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
	}

	void MyOctant::ClearEntityList(void)
	{
		//no more kids, delete the list
		for (uint i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->ClearEntityList();
		}
		m_EntityList.clear();
	}

	void MyOctant::Subdivide(void)
	{
		//if too many kids stop
		if (m_uLevel >= m_uMaxLevel) 
		{
			return;
		}
		//if you already have kids stop
		if (m_uChildren != 0)
		{
			return;
		}
	
		//make them kids

		m_uChildren = 8;

		float fSize = m_fSize / 4.0f;
		float fSizeD = fSize * 2.0f;

		vector3 v3Center;

		v3Center = m_v3Center;
		v3Center.x -= fSize;
		v3Center.y -= fSize;
		v3Center.z -= fSize;

		//create the kids stacked up into a 2x4 pile
		m_pChild[0] = new MyOctant(v3Center, fSizeD);

		v3Center.x += fSizeD;
		m_pChild[1] = new MyOctant(v3Center, fSizeD);

		v3Center.z += fSizeD;
		m_pChild[2] = new MyOctant(v3Center, fSizeD);

		v3Center.x -= fSizeD;
		m_pChild[3] = new MyOctant(v3Center, fSizeD);

		v3Center.y += fSizeD;
		m_pChild[4] = new MyOctant(v3Center, fSizeD);

		v3Center.z -= fSizeD;
		m_pChild[5] = new MyOctant(v3Center, fSizeD);

		v3Center.x += fSizeD;
		m_pChild[6] = new MyOctant(v3Center, fSizeD);

		v3Center.z += fSizeD;
		m_pChild[7] = new MyOctant(v3Center, fSizeD);

		//tell the kids who thier pap is and who the original is
		//also give them their levels then divide the kids into 8 pieces if you need more
		for (uint i = 0; i < 8; i++)
		{
			m_pChild[i]->m_pRoot = m_pRoot;
			m_pChild[i]->m_pParent = this;
			m_pChild[i]->m_uLevel = m_uLevel+1;
			if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount))
			{
				m_pChild[i]->Subdivide();
			}

		}

	}

	bool MyOctant::ContainsMoreThan(uint a_nEntities)
	{
		//check if we have more entities than idealy wanted
		uint count = 0;
		uint ObjCount = m_pEntityMngr->GetEntityCount();
		for (uint i = 0; i < ObjCount; i++)
		{
			if (IsColliding(i))
			{
				count++;
			}
			if (count>a_nEntities)
			{
				return true;
			}
		}
		return false;
	}

	void MyOctant::KillBranches(void)
	{
		//Commence order 66
		//"Master Programmer there's too many of them, what are we going to do?" ~Octree branch before the following 7 lines
		for (uint i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->KillBranches();
			delete m_pChild[i];
			m_pChild[i] = nullptr;
		}
		m_uChildren = 0;
	}

	void MyOctant::ConstructTree(uint a_nMaxLevel)
	{
		//If you're not the original then stop it, get some help
		if (m_uLevel != 0)
		{
			return;
		}

		//Let the past tree die, kill it if you have to
		m_uMaxLevel = a_nMaxLevel;

		m_uMyOctantCount = 1;

		m_EntityList.clear();

		KillBranches();
		m_lChild.clear();

		//become the tree you where meant to be
		if (ContainsMoreThan(m_uIdealEntityCount))
		{
			Subdivide();
		}

		AssignIDtoEntity();

		ConstructList();
	}

	void MyOctant::AssignIDtoEntity(void)
	{
		//give the id's
		for (uint i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->AssignIDtoEntity();
		}
		if (m_uChildren == 0)
		{
			uint entities = m_pEntityMngr->GetEntityCount();
			for (uint i = 0; i < entities; i++)
			{
				if (IsColliding(i))
				{
					m_EntityList.push_back(i);
					m_pEntityMngr->AddDimension(i,m_uID);
				}
			}
		}
	}

	uint MyOctant::GetMyOctantCount(void) { return m_uMyOctantCount; }

	void MyOctant::Release(void)
	{
		//Like a creeper blowing up my house, we're blowing up the old tree
		if (m_uLevel == 0)
		{
			KillBranches();
		}
		m_uChildren = 0;
		m_fSize = 0.0f;
		m_EntityList.clear();
		m_lChild.clear();
	}

	void MyOctant::ConstructList(void)
	{
		//we made a list, that's about it
		for (uint i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->ConstructList();
		}

		if (m_EntityList.size() > 0)
		{
			m_pRoot->m_lChild.push_back(this);
		}
	}