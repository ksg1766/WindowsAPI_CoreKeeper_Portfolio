#include "stdafx.h"
#include "PoolManager.h"
#include "Managers.h"
#include "Struct.h"
#include "Slime.h"
#include "Larva.h"
#include "BigLarva.h"
#include "Tile.h"
#include "HitEffect.h"

CPoolManager::CPoolManager()
{
}

void CPoolManager::Initialize()
{
	m_pMonsterPool = new MonsterPool;
	m_pMonsterPool->Initialize();
	m_pEffectPool = new EffectPool;
	m_pEffectPool->Initialize();
}

void CPoolManager::ReturnPool(CGameObject * _pGameObject)
{
	if(TYPE::MONSTER == _pGameObject->GetType())
		m_pMonsterPool->GetMonsterPool()->push_back(_pGameObject);
	else if(TYPE::EFFECT == _pGameObject->GetType())
		m_pEffectPool->GetEffectPool()->push_back(_pGameObject);
}

CPoolManager::~CPoolManager()
{
	Release();
}

//void CPoolManager::ReturnMonster(CGameObject * _pMonster)
//{
//	//_cMonster->SetDead(true);
//	m_pMonsterPool->GetMonsterPool()->push_back(_pMonster);
//}

void CPoolManager::Release()
{
	Safe_Delete(m_pMonsterPool);
	Safe_Delete(m_pEffectPool);
}

CPoolManager::MonsterPool::~MonsterPool()
{
	Release();
}

void CPoolManager::MonsterPool::Initialize()
{
	for (int i = 0; i < 10; ++i)
	{
		CGameObject* pSlime = new CSlime;
		pSlime->Initialize();
		m_MonsterList.push_back(pSlime);
	}
	for (int i = 0; i < 5; ++i)
	{
		CGameObject* pLarva = new CLarva;
		pLarva->Initialize();
		m_MonsterList.push_back(pLarva);
	}
	for (int i = 0; i < 5; ++i)
	{
		CGameObject* pBigLarva = new CBigLarva;
		pBigLarva->Initialize();
		m_MonsterList.push_back(pBigLarva);
	}
}

void CPoolManager::MonsterPool::CreateMonster()
{
	auto& findByState = [&](CGameObject* _iter)
	{
		if (_iter->IsDead())
			return _iter;
	};

	auto& iterMonster = find_if(m_MonsterList.begin(), m_MonsterList.end(), findByState);

	// Do Something

	(*iterMonster)->SetDead(false);

	vector<CGameObject*>& vecTile = CManagers::instance().Scene()->CurrentScene()->GetObjList(TYPE::TILE);
	UINT iTileSize = vecTile.size();
	while (true)
	{
		UINT iIndex = rand() % iTileSize;
		if (TYPE::TILE == vecTile[iIndex]->GetType())
		{
			if (dynamic_cast<CTile*>(vecTile[iIndex])->GetBiom() == dynamic_cast<CMonster*>(*iterMonster)->GetBiom())
			{
				(*iterMonster)->SetPosition(vecTile[iIndex]->GetPosition());
				break;
			}
		}
	}

	CManagers::instance().Event()->CreateObject(*iterMonster, TYPE::MONSTER);
	m_MonsterList.erase(iterMonster);
}

void CPoolManager::MonsterPool::Release()
{
	if (!(m_MonsterList.empty()))
		for (auto& iter : m_MonsterList)
		{
			Safe_Delete(iter);
		}

	m_MonsterList.clear();
}

CPoolManager::EffectPool::~EffectPool()
{
	Release();
}

void CPoolManager::EffectPool::Initialize()
{
	for (int i = 0; i < 20; ++i)
	{
		CGameObject* pHitEffect = new CHitEffect;
		pHitEffect->Initialize();
		m_EffectsList.push_back(pHitEffect);
	}
}

void CPoolManager::EffectPool::PlayEffect(EFFECT_TYPE _eEffect, Vector2 _vPos)
{
	auto& findByType = [&](CGameObject* _iter)
	{
		if (_eEffect == dynamic_cast<CEffect*>(_iter)->GetEffectType() && _iter->IsDead())
			return _iter;
	};

	auto& iterEffect = find_if(m_EffectsList.begin(), m_EffectsList.end(), findByType);
	(*iterEffect)->SetDead(false);
	(*iterEffect)->SetPosition(_vPos);
	CManagers::instance().Event()->CreateObject(*iterEffect, TYPE::EFFECT);
	m_EffectsList.erase(iterEffect);
}

void CPoolManager::EffectPool::Release()
{
	if (!(m_EffectsList.empty()))
		for (auto& iter : m_EffectsList)
		{
			Safe_Delete(iter);
		}

	m_EffectsList.clear();
}