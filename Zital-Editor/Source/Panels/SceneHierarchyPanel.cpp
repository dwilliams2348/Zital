#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "Zital/Scene/Components.h"

namespace Zital
{

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& _context)
	{
		SetContext(_context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& _context)
	{
		mContext = _context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		mContext->mRegistry.each([&](auto _entityID)
			{
				Entity entity { _entityID, mContext.get() };

				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			mSelectionContext = {};

		//right click on blank space
		ImGuiPopupFlags flags = ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems;
		if (ImGui::BeginPopupContextWindow(0, flags))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				mContext->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (mSelectionContext)
		{
			DrawComponents(mSelectionContext);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Transform"))
				{
					if(!mSelectionContext.HasComponent<TransformComponent>())
						mSelectionContext.AddComponent<TransformComponent>(glm::vec3{ 0.f, 0.f, 0.f });

					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Camera"))
				{
					if (!mSelectionContext.HasComponent<CameraComponent>())
						mSelectionContext.AddComponent<CameraComponent>();

					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite Renderer"))
				{
					if(!mSelectionContext.HasComponent<SpriteRendererComponent>())
						mSelectionContext.AddComponent<SpriteRendererComponent>();

					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity _entity)
	{
		auto& tag = _entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((mSelectionContext == _entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)_entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			mSelectionContext = _entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			//temp demo
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)56736784, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();

			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			if (mSelectionContext == _entity)
				mSelectionContext = {};

			mContext->DestroyEntity(_entity);
		}
	}

	static void DrawVec3Control(const std::string& _label, glm::vec3& _values, float _resetValue = 0.f, float _columnWidth = 100.f)
	{
		ImGui::PushID(_label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, _columnWidth);
		ImGui::Text(_label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
		ImVec2 buttonSize = { lineHeight + 3, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.25f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.f });
		if (ImGui::Button("X", buttonSize))
			_values.x = _resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &_values.x, 0.1f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.25f, 0.8f, 0.3f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });
		if (ImGui::Button("Y", buttonSize))
			_values.y = _resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &_values.y, 0.1f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.3f, 0.9f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.f });
		if (ImGui::Button("Z", buttonSize))
			_values.z = _resetValue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &_values.z, 0.1f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawComponents(Entity _entity)
	{
		/*if (_entity.HasComponent<TagComponent>())
		{
			auto& tag = _entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}

			ImGui::TreePop();
		}*/

		/*if (_entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = _entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
		}*/

		/*if (_entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cameraComponent = _entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				ImGui::Checkbox("Primary Camera", &cameraComponent.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];

						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];

							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				switch (camera.GetProjectionType())
				{
				case SceneCamera::ProjectionType::Perspective:
				{ //wont compile without curly braces error C2360
					float perspecFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &perspecFov, 0.1f)) { camera.SetPerspectiveVerticalFOV(glm::radians(perspecFov)); }

					float perspecNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near Clip", &perspecNear, 0.1f)) { camera.SetPerspectiveNearClip(perspecNear); }

					float perspecFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far Clip", &perspecFar)) { camera.SetPerspectiveFarClip(perspecFar); }
				}

					break;
				case SceneCamera::ProjectionType::Orthographic:
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize, 0.1f)) { camera.SetOrthographicSize(orthoSize); }

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near Clip", &orthoNear, 0.1f)) { camera.SetOrthographicNearClip(orthoNear); }

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip", &orthoFar, 0.1f)) { camera.SetOrthographicFarClip(orthoFar); }

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
					
					break;
				}

				ImGui::TreePop();

			}
		}*/

		/*if (_entity.HasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				auto& sprite = _entity.GetComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(sprite.Color));

				ImGui::TreePop();
			}
		}*/

		DrawUIComp<TagComponent>("Tag", _entity, [](Entity _entity)
			{
				auto& tag = _entity.GetComponent<TagComponent>().Tag;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), tag.c_str());
				if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
				{
					tag = std::string(buffer);
				}
			});
		DrawUIComp<TransformComponent>("Transform", _entity, [](Entity _entity)
			{
				auto& transform = _entity.GetComponent<TransformComponent>();
				DrawVec3Control("Translation", transform.Translation);
				glm::vec3 rotation = glm::degrees(transform.Rotation);
				DrawVec3Control("Rotation", rotation);
				transform.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", transform.Scale, 1.f);
			});
		DrawUIComp<CameraComponent>("Camera", _entity, [](Entity _entity)
			{
				auto& cameraComponent = _entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				ImGui::Checkbox("Primary Camera", &cameraComponent.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];

						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];

							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				switch (camera.GetProjectionType())
				{
				case SceneCamera::ProjectionType::Perspective:
				{ //wont compile without curly braces error C2360
					float perspecFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &perspecFov, 0.1f)) { camera.SetPerspectiveVerticalFOV(glm::radians(perspecFov)); }

					float perspecNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near Clip", &perspecNear, 0.1f)) { camera.SetPerspectiveNearClip(perspecNear); }

					float perspecFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far Clip", &perspecFar)) { camera.SetPerspectiveFarClip(perspecFar); }
				}

				break;
				case SceneCamera::ProjectionType::Orthographic:
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize, 0.1f)) { camera.SetOrthographicSize(orthoSize); }

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near Clip", &orthoNear, 0.1f)) { camera.SetOrthographicNearClip(orthoNear); }

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip", &orthoFar, 0.1f)) { camera.SetOrthographicFarClip(orthoFar); }

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);

					break;
				}
			});
		DrawUIComp<SpriteRendererComponent>("Sprite Renderer", _entity, [](Entity _entity) {
			auto& sprite = _entity.GetComponent<SpriteRendererComponent>();
			ImGui::ColorEdit4("Color", glm::value_ptr(sprite.Color));

			});
	}

}