#pragma once

#include "engine.h"
#include <assimpLoader.h>

using namespace Engine;

class TankController : public Engine::NativeScript {
public:

	TankController(entt::entity& entity, float movementSpeed, bool active) :
		NativeScript(entity),
		m_movementSpeed(movementSpeed),
		m_active(active)
	{

		entt::registry& registry = Application::getInstance().m_registry;
		Loader::ASSIMPLoad("./assets/models/Tank/shell.obj", aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate);

		m_shellVAO.reset(VertexArray::create());
		VertexBufferLayout cubeBufferLayout = { ShaderDataType::Float3,ShaderDataType::Float3 ,ShaderDataType::Float2 };
		m_shellVBO.reset(VertexBuffer::create(Loader::output.vertices.data(), sizeof(Renderer3DVertex) * Loader::output.vertices.size(), cubeBufferLayout));
		m_shellIBO.reset(IndexBuffer::create(Loader::output.indicies.data(), Loader::output.indicies.size()));
		m_shellVAO->addVertexBuffer(m_shellVBO);
		m_shellVAO->setIndexBuffer(m_shellIBO);

		std::shared_ptr<ShaderRend> shader = registry.get<RenderComponent>(entity).m_mat->getShader();

		shellMat.reset(new Material(shader, Loader::output.diffusTex, glm::vec4(.5f)));

	};

	virtual void OnUpdate(float time)
	{
		float desRotVel = 0.f;
		rp3d::Vector3 desSpeed = rp3d::Vector3(0.f, 0.f, 0.f);

		entt::registry& registry = Application::getInstance().m_registry;
		auto& rb = registry.get<RigidBodyComponent>(m_entity);
		auto& tc = registry.get<TransformComponent>(m_entity);
		auto& t = tc.GetTransform();
		rp3d::Vector3 forward(t[2][0], t[2][1], t[2][2]);
		fwd = -forward;


		if (InputPoller::isKeyPressed(NG_KEY_UP))
		{
			desSpeed = fwd * m_movementSpeed;
		}

		if (InputPoller::isKeyPressed(NG_KEY_DOWN))
		{
			desSpeed = fwd * -m_movementSpeed;
		}

		if (InputPoller::isKeyPressed(NG_KEY_LEFT))
		{
			desRotVel = 3.f;
		}

		if (InputPoller::isKeyPressed(NG_KEY_RIGHT))
		{
			desRotVel = -3.f;
		}

		float mass = rb.m_body->getMass();

		float deltaRotV = desRotVel - rb.m_body->getAngularVelocity().y;
		float rotForceY = mass * (deltaRotV / time);

		rp3d::Vector3 deltaV = desSpeed - rb.m_body->getLinearVelocity();
		rp3d::Vector3 moveForce = (deltaV / time) * mass;

		rb.m_body->applyTorque(rp3d::Vector3(0.f, rotForceY, 0.f));
		rb.m_body->applyForceToCenterOfMass(moveForce);

	}


	virtual void OnMouseBtnPressed(MouseButtonPressedEvent& e)
	{
		//projectle shooting....

		//shoot projectile with mouse click
		if (e.getButton() == 0)
		{
			std::vector<entt::entity>& m_entities = Application::getInstance().m_entities;
			entt::registry& registry = Application::getInstance().m_registry;
			bool found = false;
			int i;
			for (i = 0; i < m_entities.size(); i++)
			{
				if (m_entities[i] == entt::null)
					continue;
			}

			entt::entity projectileEntity = registry.create();
			if (i == m_entities.size())
				m_entities.push_back(projectileEntity);
			else
				m_entities[i] = projectileEntity;

			registry.emplace<LabelComponent>(projectileEntity, "Projectile");

			//auto camTransform = glm::inverse(m_camera.getCameraViewMatrix());
			auto& fireTransform = registry.get<TransformComponent>(m_entities[7]);
			auto& fireTransformMatrix = registry.get<TransformComponent>(m_entities[5]).GetTransform();
			auto& t = fireTransform.GetTransform();

			glm::vec3 Tforward(t[2][0], t[2][1], t[2][2]);

			//glm::mat4 inverted = glm::inverse(fireTransformMatrix);
			glm::vec3 forward = Tforward; // glm::vec3(fireTransformMatrix[2][0], fireTransformMatrix[2][1], fireTransformMatrix[2][2]);
			glm::vec3 camPos = { fireTransformMatrix[3][0],fireTransformMatrix[3][1] ,fireTransformMatrix[3][2] };

			auto projTransform = registry.emplace<TransformComponent>(projectileEntity, camPos - forward * 1.0f, glm::vec3(0.0f), glm::vec3(0.25f));

			registry.emplace<RenderComponent>(projectileEntity, m_shellVAO, shellMat);
			registry.emplace<RelationshipComponent>(projectileEntity);
			//HierarchySystem::setChild(m_entities[0], projectileEntity);

			auto projectiel_rb = registry.emplace<RigidBodyComponent>(projectileEntity, RigidBodyType::Dynamic, projTransform.GetTransform());
			registry.emplace<SphereColliderComponent>(projectileEntity, projectiel_rb, 0.25f);
			glm::vec3 force = (glm::vec3(fwd.x,fwd.y,fwd.z) * 500.5f);
			projectiel_rb.m_body->applyForceToCenterOfMass(rp3d::Vector3(force.x, force.y + 20.f, force.z - 1500.f));
		}
	}

	virtual void OnMouseMoved(MouseMovedEvent& e)
	{
		entt::registry& registry = Application::getInstance().m_registry;
		auto& m_tankHead = registry.get<RigidBodyComponent>(HierarchySystem::GetChildEntity(m_entity, 0));
		auto& m_tankBarrel = registry.get<RigidBodyComponent>(HierarchySystem::GetChildEntity(HierarchySystem::GetChildEntity(m_entity, 0), 0));
		float xpos = e.getX();
		float ypos = e.getY();

		if (m_firstMouse)
		{
			m_lastX = xpos;
			m_lastY = ypos;
			m_firstMouse = false;
		}

		float xoffset = xpos - m_lastX;
		float yoffset = m_lastY - ypos;
		m_lastX = xpos;
		m_lastY = ypos;

		float sensitivity = 0.5f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_yaw += xoffset;
		m_pitch += yoffset;

		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;

		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

		if (xoffset > 0.5f)
		{
			glm::quat quatX = glm::angleAxis(glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
			glm::quat quatY = glm::angleAxis(glm::radians(10.f), glm::vec3(0.f, 1.f, 0.f));
			glm::quat quatZ = glm::angleAxis(glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));

			//m_tankHead.rotation = quatX + quatY + quatZ;
			//m_tankHead.rotation += glm::quat(0, 0, 2.0f, 0);
			m_tankHead.m_body->setAngularVelocity(rp3d::Vector3(0, -1.f, 0.0f));
			m_tankHead.m_body->setAngularDamping(0.9f);
		}
		else if (xoffset < -0.5f)
		{
			//m_tankHead.rotation += glm::quat(0, 0, -2.0f, 0);
			m_tankHead.m_body->setAngularVelocity(rp3d::Vector3(0, 1.f, 0.0f));
			m_tankHead.m_body->setAngularDamping(0.9f);
		}


		if (yoffset > 0.8f)
		{
			m_tankBarrel.m_body->setAngularVelocity(rp3d::Vector3(0.5f, 0.0f, 0.0f));
			m_tankBarrel.m_body->setAngularDamping(0.9f);

		}
		else if (yoffset < -0.8f)
		{
			m_tankBarrel.m_body->setAngularVelocity(rp3d::Vector3(-0.5f, 0.0f, 0.0f));
			m_tankBarrel.m_body->setAngularDamping(0.9f);
		}
		rp3d::Transform t = m_tankBarrel.m_body->getTransform();


		if (m_tankBarrel.m_body->getTransform().getOrientation().x > 0.12f)
		{
			Log::error("Clamping,,...");
			rp3d::Vector3 ve = { 0.12f,0,0 };
			rp3d::Quaternion quat = { 0,ve };
			t.setOrientation(quat);
		}
		else if (m_tankBarrel.m_body->getTransform().getOrientation().x < 0.000f)
		{
			rp3d::Vector3 ve = { 0,0,0 };
			rp3d::Quaternion quat = { ve, 0 };
			t.setOrientation(quat);
		}

		Log::warn(m_tankBarrel.m_body->getTransform().getOrientation().x);

	}

	virtual void OnKeyPressed(KeyPressedEvent& e)
	{
		entt::registry& registry = Application::getInstance().m_registry;
		auto& m_tankRB = registry.get<RigidBodyComponent>(m_entity);

		//if (e.getKeyCode() == NG_KEY_UP)
		//{
		//	m_tankRB.m_body->applyForceToCenterOfMass(rp3d::Vector3(0.0f, 0.0f, -25.0f));
		//}
		//if (e.getKeyCode() == NG_KEY_DOWN)
		//{
		//	m_tankRB.m_body->applyForceToCenterOfMass(rp3d::Vector3(0.0f, 0.0f, 25.0f));
		//}
		//if (e.getKeyCode() == NG_KEY_LEFT)
		//{
		//	m_tankRB.m_body->applyForceToCenterOfMass(rp3d::Vector3(-25.0f, 0.0f, 0.0f));
		//}
		//if (e.getKeyCode() == NG_KEY_RIGHT)
		//{
		//	m_tankRB.m_body->applyForceToCenterOfMass(rp3d::Vector3(25.0f, 0.0f, 0.0f));
		//}

	}

protected:
	float m_movementSpeed;
	float m_timeElapsed;
	bool m_active = true;
	std::shared_ptr<VertexArray> m_shellVAO;
	std::shared_ptr<VertexBuffer> m_shellVBO;
	std::shared_ptr<IndexBuffer> m_shellIBO;
	std::shared_ptr<Material> shellMat;
	rp3d::Vector3 fwd;

	bool m_firstMouse = true;
	float m_lastX;
	float m_lastY;
	float m_yaw = -90.0f;	//!< initialized m_yaw as -90.0 to rotate to the left.
	float m_pitch = 0.0f; //!< m_pitch for mouse movement
	glm::vec3 front;

};