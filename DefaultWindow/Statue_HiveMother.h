#pragma once
#include "Wall.h"
class CStatue_HiveMother :
	public CWall
{
public:
	CStatue_HiveMother();
	virtual ~CStatue_HiveMother();

	// CGameObject��(��) ���� ��ӵ�
	virtual void Initialize(void) override;
	virtual int	 Update(void) override;
	virtual int	 LateUpdate(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

	virtual void	OnCollisionStay(CCollider* _pOther) override;

private:
	void		SetMotion(void);
	void		MoveFrame(void);
};