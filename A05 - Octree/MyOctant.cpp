#include "MyOctant.h"

Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
}

Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
}

Simplex::MyOctant::MyOctant(MyOctant const& other)
{
}

MyOctant& Simplex::MyOctant::operator=(MyOctant const& other)
{
	// TODO: insert return statement here
}

Simplex::MyOctant::~MyOctant(void)
{
}

void Simplex::MyOctant::Swap(MyOctant& other)
{
}

float Simplex::MyOctant::GetSize(void){return m_fSize;}

vector3 Simplex::MyOctant::GetCenterGlobal(void){return m_v3Center;}

vector3 Simplex::MyOctant::GetMinGlobal(void){return m_v3Min;}

vector3 Simplex::MyOctant::GetMaxGlobal(void){return m_v3Max;}

bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
{
	return false;
}

void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
}

void Simplex::MyOctant::Display(vector3 a_v3Color)
{
}

void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
}

void Simplex::MyOctant::ClearEntityList(void)
{
}

void Simplex::MyOctant::Subdivide(void)
{
}

MyOctant* Simplex::MyOctant::GetChild(uint a_nChild)
{
	return nullptr;
}

MyOctant* Simplex::MyOctant::GetParent(void)
{
	return nullptr;
}

bool Simplex::MyOctant::IsLeaf(void)
{
	return false;
}

bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities)
{
	return false;
}

void Simplex::MyOctant::KillBranches(void)
{
}

void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{
}

void Simplex::MyOctant::AssignIDtoEntity(void)
{
}

uint Simplex::MyOctant::GetMyOctantCount(void)
{
	return uint();
}
