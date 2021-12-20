#include "Component.h"
#include "Transform.h"
#include "GameObject.h"
#include "EngineData.h"
#include "DebugManager.h"
#include "Rigidbody.h"

Transform::Transform()
{
	Position = { 0,0,0 };
	Rotation = { 0,0,0 };
	Scale = { 1,1,1 };
	World_M = DirectX::XMMatrixIdentity();

	Local_UP	= { 0,0,0 };
	Local_Right = { 0,0,0 };
	Local_Look	= { 0,0,0 };

	LocalUpdate = false;

	Load_World = DirectX::XMMatrixIdentity();
	Load_Local = DirectX::XMMatrixIdentity();

	isRigid = false;
}

Transform::~Transform()
{

}

void Transform::Start()
{
	//물리충돌된 좌표를 받는다
	if (gameobject->GetComponent<Rigidbody>() != nullptr ) 
	{
		isRigid = true;
	}

}

void Transform::TransformUpdate()
{
	//월드 좌표들을 기반으로 월드 행렬을 구한다
	//gameobject->Name;
	UpdateWorldXM();

	UpdateLocalPosition();

	//업데이트가 끝난후 오브젝트 안에 매쉬데이터를 업데이트
	gameobject->OneMeshData->mWorld = *(GetWorld());
}

DirectX::SimpleMath::Vector3 Transform::GetLocalPosition_UP()
{
	return Local_UP;
}

DirectX::SimpleMath::Vector3 Transform::GetLocalPosition_Right()
{
	return Local_Right;
}

DirectX::SimpleMath::Vector3 Transform::GetLocalPosition_Look()
{
	return Local_Look;
}

void Transform::SetLocalPosition(float X, float Y, float Z)
{
	Position =
	{
		Position.x + (Local_Right.x * X),
		Position.y + (Local_Right.y * X),
		Position.z + (Local_Right.z * X)
	};

	Position =
	{
		Position.x + (Local_UP.x * Y),
		Position.y + (Local_UP.y * Y),
		Position.z + (Local_UP.z * Y)
	};


	Position =
	{
		Position.x + (Local_Look.x * Z),
		Position.y + (Local_Look.y * Z),
		Position.z + (Local_Look.z * Z)
	};
}

void Transform::SetTranlate(float X, float Y, float Z)
{
	Position.x += X;
	Position.y += Y;
	Position.z += Z;
}

void Transform::SetTranlate(DirectX::SimpleMath::Vector3 mPos)
{
	Position.x += mPos.x;
	Position.y += mPos.y;
	Position.z += mPos.z;
}

void Transform::SetRotate(float X, float Y, float Z)
{
	Rotation.x += X;
	Rotation.y += Y;
	Rotation.z += Z;
}

void Transform::SetRotate(DirectX::SimpleMath::Vector3 mRot)
{
	Rotation.x += mRot.x;
	Rotation.y += mRot.y;
	Rotation.z += mRot.z;
}

void Transform::SetScale(float X, float Y, float Z)
{
	Scale.x += X;
	Scale.y += Y;
	Scale.z += Z;
}

void Transform::SetScale(DirectX::SimpleMath::Vector3 mScl)
{
	Scale.x += mScl.x;
	Scale.y += mScl.y;
	Scale.z += mScl.z;
}

DirectX::SimpleMath::Matrix Transform::GetPositionXM()
{
	return PositionXM;
}

DirectX::SimpleMath::Matrix Transform::GetRotationXM()
{
	return RotationXM;
}

DirectX::SimpleMath::Matrix Transform::GetScaleXM()
{
	return ScaleXM;
}

DirectX::SimpleMath::Matrix* Transform::GetWorld()
{
	return &World_M;
}

void Transform::SetLocalUpdate(bool isUpdate)
{
	LocalUpdate = isUpdate;
}

void Transform::SetChild(Transform* mChild)
{
	ChildList.push_back(mChild);
}

void Transform::SetParnet(Transform* mParent)
{
	Parent = mParent;
}

void Transform::Child_Local_Updata()
{
	if (Parent == nullptr)
	{
		Load_Local = Load_World;
	}
	else
	{
		//DirectX::XMMATRIX TM2_1 = DirectX::XMMatrixInverse(nullptr, Parent->Load_World);
		Load_Local = Load_World * Parent->Load_Local;
	}

	for (int i = 0; i < ChildList.size(); i++) 
	{
		ChildList[i]->Child_Local_Updata();
	}
}

DirectX::SimpleMath::Matrix Transform::CreateXMPos4x4()
{
	DirectX::SimpleMath::Matrix Position_4x4;
	Position_4x4._11 = 1;				Position_4x4._12 = 0;			Position_4x4._13 = 0;			Position_4x4._14 = 0;
	Position_4x4._21 = 0;				Position_4x4._22 = 1;			Position_4x4._23 = 0;			Position_4x4._24 = 0;
	Position_4x4._31 = 0;				Position_4x4._32 = 0;			Position_4x4._33 = 1;			Position_4x4._34 = 0;
	Position_4x4._41 = Position.x;		Position_4x4._42 = Position.y;	Position_4x4._43 = Position.z;	Position_4x4._44 = 1;

	return Position_4x4;
}

DirectX::SimpleMath::Matrix Transform::CreateXMRot4x4()
{
	float radX = Rotation.x * 3.141592f / 180;
	float radY = Rotation.y * 3.141592f / 180;
	float radZ = Rotation.z * 3.141592f / 180;
	DirectX::XMMATRIX _P = DirectX::XMMatrixRotationX(radX);
	DirectX::XMMATRIX _Y = DirectX::XMMatrixRotationY(radY);
	DirectX::XMMATRIX _R = DirectX::XMMatrixRotationZ(radZ);

	DirectX::XMMatrixRotationRollPitchYaw(radX, radY, radZ);

	return _R * _Y * _P;
}

DirectX::SimpleMath::Matrix Transform::CreateXMRot4x4_Q()
{
	return XMMatrixRotationQuaternion(Q_Rotation);
}

DirectX::SimpleMath::Matrix Transform::CreateXMScl4x4()
{
	DirectX::SimpleMath::Matrix Scale_4x4;
	Scale_4x4._11 = Scale.x;	Scale_4x4._12 = 0;			Scale_4x4._13 = 0;			Scale_4x4._14 = 0;
	Scale_4x4._21 = 0;			Scale_4x4._22 = Scale.y;	Scale_4x4._23 = 0;			Scale_4x4._24 = 0;
	Scale_4x4._31 = 0;			Scale_4x4._32 = 0;			Scale_4x4._33 = Scale.z;	Scale_4x4._34 = 0;
	Scale_4x4._41 = 0;			Scale_4x4._42 = 0;			Scale_4x4._43 = 0;			Scale_4x4._44 = 1;

	return Scale_4x4;
}

void Transform::UpdateWorldXM()
{
	PositionXM	= CreateXMPos4x4();
	ScaleXM		= CreateXMScl4x4();

	if (isRigid == false)
	{
		RotationXM	= CreateXMRot4x4();
	}
	else
	{
		RotationXM = CreateXMRot4x4_Q();
	}

	DirectX::SimpleMath::Matrix Master = (ScaleXM * RotationXM * PositionXM);
	if (Parent != nullptr)
	{
		World_M = Load_Local * Master * Parent->World_M;
	}
	else
	{
		World_M = Load_Local*  Master;
	}
}

void Transform::UpdateLocalPosition()
{
	//월드 행렬 구하기
	DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(World_M);
	//로컬좌표를 구하기위해 월드의 역행렬을 구함
	DirectX::SimpleMath::Matrix A_Master4x4 = DirectX::XMMatrixInverse(&det, World_M);
	
	//각각의 값을 넣어준다
	Local_Right.x = A_Master4x4._11;
	Local_Right.y = A_Master4x4._12;
	Local_Right.z = A_Master4x4._13;

	Local_UP.x = A_Master4x4._21;
	Local_UP.y = A_Master4x4._22;
	Local_UP.z = A_Master4x4._23;

	Local_Look.x = A_Master4x4._31;
	Local_Look.y = A_Master4x4._32;
	Local_Look.z = A_Master4x4._33;
}






