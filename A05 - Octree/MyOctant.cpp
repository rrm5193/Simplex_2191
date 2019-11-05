#include "MyOctant.h"

using namespace Simplex;
	void Simplex::MyOctant::Init(void)
	{
		m_uMyOctantCount = 0;
		m_uMaxLevel = 3;
		m_uIdealEntityCount = 5;
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

	void Simplex::MyOctant::Swap(MyOctant& other)
	{
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

		std::swap(m_fSize, other.m_fSize);
		std::swap(m_fSize, other.m_fSize);

		for (uint i = 0; i < 8; i++) {
			std::swap(m_pChild[i], other.m_pChild[i]);
		}

	}

	Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
	{
		Init();
		m_uMyOctantCount = 0;
		m_uMaxLevel = a_nMaxLevel;
		m_uIdealEntityCount = a_nIdealEntityCount;
		m_uID = m_uMyOctantCount;

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
		vector3 v3Center = pRigidBody->GetCenterLocal();
		lMinMax.clear();
		SafeDelete(pRigidBody);

		m_fSize = fMax * 2.0f;
		m_v3Center = v3Center;
		m_v3Min = m_v3Center - (vector3(fMax));
		m_v3Max = m_v3Center + (vector3(fMax));
	}

	Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
	{
		Init();
		m_v3Center = a_v3Center;
		m_fSize = a_fSize;

		m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
		m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);

		m_uMyOctantCount++;

	}

	Simplex::MyOctant::MyOctant(MyOctant const& other)
	{
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
		if (this != &other) {
			Release();
			Init();
			MyOctant temp(other);
			Swap(temp);
		}
	}

	Simplex::MyOctant::~MyOctant(void){Release();}

	float Simplex::MyOctant::GetSize(void) { return m_fSize; }

	vector3 Simplex::MyOctant::GetCenterGlobal(void) { return m_v3Center; }

	vector3 Simplex::MyOctant::GetMinGlobal(void) { return m_v3Min; }

	vector3 Simplex::MyOctant::GetMaxGlobal(void) { return m_v3Max; }

	bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
	{
		uint nObjectCount = m_pEntityMngr->GetEntityCount();

		if (a_uRBIndex >= nObjectCount)
		{
			return false;
		}

		MyEntity* entityPointer = m_pEntityMngr->GetEntity(a_uRBIndex);
		MyRigidBody* rigBody = entityPointer->GetRigidBody();
		vector3 v3MinO = rigBody->GetMinGlobal();
		vector3 v3MaxO = rigBody->GetMaxGlobal();

		if (m_v3Max.x < v3MinO.x || m_v3Max.x > v3MaxO.x || m_v3Max.y < v3MinO.y || m_v3Max.y > v3MaxO.y || m_v3Max.z < v3MinO.z || m_v3Max.z > v3MaxO.z) {
			return false;
		}

		return true;

	}

	void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
	{
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

	void Simplex::MyOctant::Display(vector3 a_v3Color)
	{
		for (uint nIndex = 0; nIndex < m_uChildren; nIndex++)
		{
			m_pChild[nIndex]->Display(a_v3Color);
		}
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
	}

	void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
	{
		uint nLeafs = m_lChild.size();
		for (uint i = 0; i < nLeafs; i++)
		{
			m_lChild[i]->DisplayLeafs(a_v3Color);
		}
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center)*glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
	}

	void Simplex::MyOctant::ClearEntityList(void)
	{
		for (uint i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->ClearEntityList();
		}
		m_EntityList.clear();
	}

	void Simplex::MyOctant::Subdivide(void)
	{
		if (m_uLevel >= m_uMaxLevel || m_uChildren != 0)
		{
			return;
		}

		m_uChildren = 8;

		float fSize = m_fSize / 4.0f;
		float fSizeD = fSize * 2.0f;

		vector3 v3Center;

		v3Center = m_v3Center;
		v3Center.x -= fSize;
		v3Center.y -= fSize;
		v3Center.z -= fSize;
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

	MyOctant* Simplex::MyOctant::GetChild(uint a_nChild)
	{
		if (a_nChild > 7)
		{
			return nullptr;
		}
		return m_pChild[a_nChild];
	}

	MyOctant* Simplex::MyOctant::GetParent(void){return m_pParent;}

	bool Simplex::MyOctant::IsLeaf(void){return m_uChildren==0;}

	bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities)
	{
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

	void Simplex::MyOctant::KillBranches(void)
	{
		for (uint i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->KillBranches();
			delete m_pChild[i];
			m_pChild[i] = nullptr;
		}
		m_uChildren = 0;
	}

	void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
	{
		if (m_uLevel != 0)
		{
			return;
		}

		m_uMaxLevel = a_nMaxLevel;

		m_uMyOctantCount = 1;

		m_EntityList.clear();

		KillBranches();
		m_lChild.clear();

		if (ContainsMoreThan(m_uIdealEntityCount))
		{
			Subdivide();
		}

		AssignIDtoEntity();

		ConstructList();
	}

	void Simplex::MyOctant::AssignIDtoEntity(void)
	{
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

	uint Simplex::MyOctant::GetMyOctantCount(void) { return m_uMyOctantCount; }

	void Simplex::MyOctant::Release(void)
	{
		if (m_uLevel == 0)
		{
			KillBranches();
		}
		m_uChildren = 0;
		m_fSize = 0.0f;
		m_EntityList.clear();
		m_lChild.clear();
	}

	void Simplex::MyOctant::ConstructList(void)
	{
		for (uint i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->ConstructList();
		}

		if (m_EntityList.size() > 0)
		{
			m_pRoot->m_lChild.push_back(this);
		}
	}