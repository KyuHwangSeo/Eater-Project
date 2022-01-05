#include "MainHeader.h"
#include "inGame.h"
#include "GameObject.h"
#include "MeshFilter.h"
#include "SkinningFilter.h"
#include "AnimationController.h"
#include "Transform.h"
#include "AI.h"
#include "Player.h"
#include "KeyInput.h"
#include "Camera.h"
#include "Light.h"
#include "Terrain.h"
#include "ParticleSystem.h"

void InGame::Awake()
{
	//불러오는 매쉬의 경로 파악
	LoadMeshPath("../Resources/Mesh/");
	LoadTexturePath("../Resources/Texture/");

	LoadMesh("IceSpikes22", SCALING | ROTATE_AXIS);

	//LoadMesh("Dome", SCALING);
	//LoadMesh("Inside", SCALING);
	LoadMesh("Player_Run");
	LoadMesh("Player_Roll", ANIMATION_ONLY);
	LoadMesh("Player_Idle", ANIMATION_ONLY);
	LoadMesh("Pistol", SCALING);
	LoadMesh("Weapon", SCALING);

	LoadMesh("TerrainDecimate", SCALING);
	LoadTerrainMesh("Terrain", "Terrain_RGB.png", SCALING);

	LoadTexture("Player.dds"); 
	LoadTexture("Dump.png");

	LoadTexture("ground01_Albedo.png");
	LoadTexture("ground02_Albedo.png");
	LoadTexture("ground01_Normal.png");
	LoadTexture("ground02_Normal.png");

	LoadTexture("Particle_blast.png");
	LoadTexture("Particle_blast2.png");
	LoadTexture("Particle_embers4.png");
	LoadTexture("Particle_hotCloudColor.png");

	LoadTexture("Particle_hotCloud.png");
	LoadTexture("Particle_blast4.png");
	LoadTexture("Particle_glowSphere.png");
	LoadTexture("Particle_glowSphere2.png");
	LoadTexture("Particle_explosion.png");
	LoadTexture("Particle_ball88.png");
	LoadTexture("Particle_stand88.png");
	LoadTexture("Particle_glowBubbles.png");

	LoadTexture("Particle_Slash_1.png");
	LoadTexture("Particle_Slash_2.png");
	LoadTexture("Particle_Smoke_1.png");

	LoadTexture("Particle_Smoke_7.png");

	///카메라
	testobj = Instance("Cam");
	testobj->AddComponent<Camera>();
	testobj->AddComponent<Keyinput>();
	testobj->GetTransform()->Position = { 0,5,-25 };

	///라이트
	testobj = Instance("DirectionLight");
	testobj->AddComponent<DirectionLight>();

	///터레인
	testobj = InstanceTerrain("Terrain");
	Terrain* terrain = testobj->GetComponent<Terrain>();
	terrain->SetMeshName("Terrain");
	terrain->SetLayerName("ground01_Albedo", "ground01_Normal");
	terrain->SetLayerName("ground02_Albedo", "ground02_Normal");
	terrain->SetColliderName("TerrainDecimate");
	terrain->SetTextureTiling(1.0f / 31.0f);
	testobj->GetTransform()->Rotation = { 0, 0, 0 };

	//testobj = Instance("Player");
	//testobj->AddComponent<AnimationController>();
	//testobj->AddComponent<MeshFilter>();
	//testobj->AddComponent<Player>();


	//testobj = Instance("Dome");
	//testobj->AddComponent<MeshFilter>()->SetMeshName("Dome");
	//
	//testobj = Instance("Inside");
	//testobj->AddComponent<MeshFilter>()->SetMeshName("Inside");

	
	ParticleObj = Instance();




	testobj = InstanceParticle("Particle_0");
	ParticleObj->ChoiceChild(testobj);

	ParticleSystem* particles0 = testobj->GetComponent<ParticleSystem>();
	particles0->SetMeshName("Quad");
	particles0->SetRenderType(PARTICLE_RENDER_OPTION::MESH);
	particles0->SetDiffuseName("Particle_Slash_2");
	particles0->SetStartLifeTime(1.5f);
	particles0->SetStartScale(10.0f);
	particles0->SetStartRotation(-90);
	particles0->SetStartColor(Vector4(192, 192, 48, 255));
	particles0->SetMaxParticles(10);
	particles0->SetRateOverTime(5.0f);
	particles0->SetShapeRadius(0.0f);
	particles0->SetStartForce(Vector3(-10, 0, 0));
	particles0->SetLifeTimeForce(Vector3(0, 0, 0), Vector3(0, 0, 0));
	particles0->SetLifeTimeColor(Vector4(255, 255, 0, 0), Vector4(255, 255, 255, 255), PARTICLE_LIFETIME_OPTION::UPDOWN);
	particles0->SetLifeTimeScale(0.0f, 1.0f, PARTICLE_LIFETIME_OPTION::UP);
	particles0->SetLifeTimeRotation(0.0f);
	particles0->SetTextureTiling(8, 8);
	particles0->SetPlayTime(2.0f);
	ParticleList.push_back(particles0);

	testobj = InstanceParticle("Particle_1");
	testobj->GetTransform()->Position.y += 1.0f;
	ParticleObj->ChoiceChild(testobj);

	ParticleSystem* particles1 = testobj->GetComponent<ParticleSystem>();
	particles1->SetMeshName("Quad");
	particles1->SetRenderType(PARTICLE_RENDER_OPTION::HORIZONTAL_BILLBOARD);
	particles1->SetDiffuseName("Particle_Slash_2");
	particles1->SetStartLifeTime(3.0f);
	particles1->SetStartScale(5.0f);
	particles1->SetStartRotation(-360, 360);
	particles1->SetStartColor(Vector4(0, 192, 48, 255));
	particles1->SetMaxParticles(30);
	particles1->SetRateOverTime(5.0f);
	particles1->SetShapeRadius(0.01f);
	particles1->SetStartForce(Vector3(0, 0, 0));
	particles1->SetLifeTimeForce(Vector3(0, 0, 0), Vector3(0, 0, 0));
	particles1->SetLifeTimeColor(Vector4(0, 0, 0, 0), Vector4(0, 255, 255, 255), PARTICLE_LIFETIME_OPTION::UPDOWN);
	//particles1->SetLifeTimeScale(0.0f, 1.0f, PARTICLE_LIFETIME_OPTION::UPDOWN);
	particles1->SetLifeTimeRotation(360.0f);
	particles1->SetTextureTiling(8, 8);
	particles1->SetPlayTime(2.0f);
	ParticleList.push_back(particles1);

	testobj = InstanceParticle("Particle_2");
	ParticleObj->ChoiceChild(testobj);

	particles = testobj->GetComponent<ParticleSystem>();
	particles->SetMeshName("Quad");
	particles->SetRenderType(PARTICLE_RENDER_OPTION::HORIZONTAL_BILLBOARD);
	particles->SetDiffuseName("Particle_Slash_2");
	particles->SetStartLifeTime(1.5f);
	particles->SetStartScale(10.0f);
	particles->SetStartRotation(-90);
	particles->SetStartColor(Vector4(192, 30 , 48, 255));
	particles->SetMaxParticles(6);
	particles->SetRateOverTime(3.0f);
	particles->SetShapeRadius(0.01f);
	particles->SetStartForce(Vector3(-20, 0, 0));
	particles->SetLifeTimeForce(Vector3(0, 0, 0), Vector3(0, 0, 0));
	particles->SetLifeTimeColor(Vector4(255, 0, 0, 0), Vector4(255, 255, 255, 255), PARTICLE_LIFETIME_OPTION::UPDOWN);
	particles->SetLifeTimeScale(0.0f, 1.0f, PARTICLE_LIFETIME_OPTION::UP);
	particles->SetLifeTimeRotation(0.0f);
	particles->SetTextureTiling(8, 8);
	particles->SetPlayTime(2.0f);
	particles->SetNextParticle(particles0);
	particles->SetNextParticle(particles1);
	ParticleList.push_back(particles);
}

void InGame::Start()
{
}

void InGame::Update()
{
	if (GetKey(VK_LEFT) == true)
	{
		ParticleObj->GetTransform()->Position.x -= 10.0f * GetDeltaTime();
	}
	if (GetKey(VK_RIGHT) == true)
	{
		ParticleObj->GetTransform()->Position.x += 10.0f * GetDeltaTime();
	}

	if (GetKey(VK_UP) == true)
	{
		ParticleObj->GetTransform()->Position.z += 10.0f * GetDeltaTime();
	}
	if (GetKey(VK_DOWN) == true)
	{
		ParticleObj->GetTransform()->Position.z -= 10.0f * GetDeltaTime();
	}

	//if (GetKeyUp(VK_RETURN))
	//{
	//	for (ParticleSystem* p : ParticleList)
	//	{
	//		p->SetPlay(10.f, true);
	//	}
	//}

	if (GetKeyUp(VK_F1))
	{
		particles->Play();
	}

	if (GetKeyUp(VK_F2))
	{
		for (ParticleSystem* p : ParticleList)
		{
			p->Play();
		}
	}

	if (GetKeyUp(VK_F3))
	{
		for (ParticleSystem* p : ParticleList)
		{
			p->Play(true);
		}
	}
}

void InGame::End()
{
}
