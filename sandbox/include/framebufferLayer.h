/** framebufferLayer.h */
#pragma once

#include "engine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "behaviorScripts/tankController.h"
#include <assimpLoader.h>

namespace Engine {

	class FramebufferLayer : public Layer {
	public:
		FramebufferLayer(const char* name);
		void OnUpdate(float timestep) override;
		void OnRender() override;
		void onMouseMoved(MouseMovedEvent& e) override;
		void onKeyPressed(KeyPressedEvent& e) override;
	private:
		//vertices adata
		float cubeVertices[8 * 24] = {
			//	 <------ Pos ------>  <--- normal --->  <-- UV -->
				 0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  0.f,  0.f,
				 0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  0.f,  0.5f,
				-0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  0.33f,0.5f,
				-0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  0.33f,0.f,

				-0.5f, -0.5f, 0.5f,   0.f,  0.f,  1.f,  0.33f,0.5f,
				 0.5f, -0.5f, 0.5f,   0.f,  0.f,  1.f,  0.66f,0.5f,
				 0.5f,  0.5f, 0.5f,   0.f,  0.f,  1.f,  0.66f,0.f,
				-0.5f,  0.5f, 0.5f,   0.f,  0.f,  1.f,  0.33, 0.f,

				-0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  1.f,  0.f,
				 0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  0.66f,0.f,
				 0.5f, -0.5f, 0.5f,   0.f, -1.f,  0.f,  0.66f,0.5f,
				-0.5f, -0.5f, 0.5f,   0.f, -1.f,  0.f,  1.0f, 0.5f,

				0.5f,  0.5f, 0.5f,   0.f,  1.f,  0.f,	0.f,  0.5f,
				 0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  0.f,  1.0f,
				-0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  0.33f,1.0f,
				-0.5f,  0.5f, 0.5f,   0.f,  1.f,  0.f,  0.3f, 0.5f,

				-0.5f,  0.5f, 0.5f,  -1.f,  0.f,  0.f,  0.66f,0.5f,
				-0.5f,  0.5f, -0.5f, -1.f,  0.f,  0.f,  0.33f,0.5f,
				-0.5f, -0.5f, -0.5f, -1.f,  0.f,  0.f,  0.33f,1.0f,
				-0.5f, -0.5f, 0.5f,  -1.f,  0.f,  0.f,  0.66f,1.0f,

				0.5f, -0.5f, -0.5f,  1.f,  0.f,  0.f,  1.0f,  1.0f,
				0.5f,  0.5f, -0.5f,  1.f,  0.f,  0.f,  1.0f,  0.5f,
				0.5f,  0.5f, 0.5f,   1.f,  0.f,  0.f,  0.66f, 0.5f,
				0.5f, -0.5f, 0.5f,   1.f,  0.f,  0.f,  0.66f, 1.0f
		};



		float pyramidVertices[8 * 16] = {
			// < --- Pos--- >		< ---normal--->			<--UV--> 
			-0.5f, -0.5f, -0.5f,	0.f, -1.f, 0.f,			0.f, 0.f,
			0.5f, -0.5f, -0.5f,		0.f, -1.f, 0.f,			0.f, 0.5f,
			0.5f, -0.5f, 0.5f,		0.f, -1.f, 0.f,			0.33f, 0.5f,
			-0.5f, -0.5f, 0.5f,		0.f, -1.f, 0.f,			0.33f, 0.f,

			-0.5f, -0.5f, -0.5f,	-0.8944f, 0.4472f, 0.f,	0.f, 0.f,
			-0.5f, -0.5f, 0.5f,		-0.8944f, 0.4472f, 0.f,	0.f, 0.f,
			0.0f, 0.5f, 0.0f,		-0.8944f, 0.4472f, 0.f, 0.f, 0.f,

			-0.5f, -0.5f,  0.5f,	0.f, 0.4472f, 0.8944f,	0.f, 0.f,
			 0.5f, -0.5f,  0.5f,	0.f, 0.4472f, 0.8944f,	0.f, 0.f,
			 0.0f,  0.5f,  0.0f,	0.f, 0.4472f, 0.8944f,	0.f, 0.f,

			 0.5f, -0.5f,  0.5f,	0.8944f, 0.4472f, 0.f,	0.f, 0.f,
			 0.5f, -0.5f, -0.5f,	0.8944f, 0.4472f, 0.f,	0.f, 0.f,
			 0.0f,  0.5f,  0.0f,	0.8944f, 0.4472f, 0.f,	0.f, 0.f,

			 0.5f, -0.5f, -0.5f,	0.f, 0.4472f, -0.8944f,	0.f, 0.f,
			-0.5f, -0.5f, -0.5f,	0.f, 0.4472f, -0.8944f, 0.f, 0.f,
			 0.0f,  0.5f,  0.0f,	0.f, 0.4472f, -0.8944f, 0.f, 0.f,

		};

		uint32_t pyramidIndices[3 * 6] =
		{
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			7, 8, 9,
			10, 11, 12,
			13, 14, 15
		};

		uint32_t cubeIndices[3 * 12] = {
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			6, 7, 4,
			8, 9, 10,
			10, 11, 8,
			12, 13, 14,
			14, 15, 12,
			16, 17, 18,
			18, 19, 16,
			20, 21, 22,
			22, 23, 20
		};
		//


		std::shared_ptr<VertexArray> m_VAO1, m_VAO2;
		std::shared_ptr<VertexBuffer> m_VBO1, m_VBO2;
		std::shared_ptr<IndexBuffer> m_IBO1, m_IBO2;
		std::shared_ptr<ShaderRend> shader;
		std::shared_ptr<Material> mat1, mat2;
		std::shared_ptr<Material> wireframeMat;

		glm::mat4 m_view3D,m_view2D;
		glm::mat4 m_projection3D, m_projection2D;
		using SceneWideUniforms = std::unordered_map<const char*, std::pair<ShaderDataType, void*>>; //!< sceen wide uniform 
		SceneWideUniforms m_swu3D, m_swu2D;
		glm::mat4 m_model1, m_model2;

		entt::registry& m_registry; //ESC registry whatever that is
		std::vector<entt::entity>& m_entities; //Entities

		entt::entity m_currentCamera; //Camera thing

		Camera m_camera;

		std::shared_ptr<RendererCommands> clearColorAndDepthCommand;
		std::shared_ptr<RendererCommands> setGlLineCmd;
		std::shared_ptr<RendererCommands> setGlFillCmd;

		std::shared_ptr<RendererCommands> enableBlendCommand;
		std::shared_ptr<RendererCommands> disableDepthCommand;
		std::shared_ptr<RendererCommands> enableDepthCommand;
		std::shared_ptr<RendererCommands> disableBlendCommand;
		std::shared_ptr<RendererCommands> standardBlend;

		std::shared_ptr<Framebuffer> defaultTarget;
		std::shared_ptr<Framebuffer> textureTarget;
		std::shared_ptr<Framebuffer> depthTarget;

		Quad m_screenQuad;
		SubTexture m_screenTexture;
		bool usePP = false;
	};
}
