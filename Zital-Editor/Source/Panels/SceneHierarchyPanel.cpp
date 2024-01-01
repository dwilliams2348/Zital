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
		mSelectionContext = {};
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
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity _entity)
	{
		mSelectionContext = _entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity _entity)
	{
		auto& tag = _entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((mSelectionContext == _entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_SpanAvailWidth;
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
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
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
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

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
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			_values.x = _resetValue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &_values.x, 0.1f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.25f, 0.8f, 0.3f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			_values.y = _resetValue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &_values.y, 0.1f, 0.f, 0.f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.3f, 0.9f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			_values.z = _resetValue;
		ImGui::PopFont();

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
		//no need for if check since all new entities all get initialized with a tag component already attached
		auto& tag = _entity.GetComponent<TagComponent>().Tag;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), tag.c_str());
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}


		ImGui::SameLine();
		ImGui::PushItemWidth(-1.f);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Transform"))
			{
				if (!mSelectionContext.HasComponent<TransformComponent>())
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
				if (!mSelectionContext.HasComponent<SpriteRendererComponent>())
					mSelectionContext.AddComponent<SpriteRendererComponent>();

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", _entity, [](auto& _component)
			{
				DrawVec3Control("Translation", _component.Translation);
				glm::vec3 rotation = glm::degrees(_component.Rotation);
				DrawVec3Control("Rotation", rotation);
				_component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", _component.Scale, 1.f);
			});
		DrawComponent<CameraComponent>("Camera", _entity, [](auto& _component)
			{
				auto& camera = _component.Camera;

				ImGui::Checkbox("Primary Camera", &_component.Primary);

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

					ImGui::Checkbox("Fixed Aspect Ratio", &_component.FixedAspectRatio);

					break;
				}
			});
		DrawComponent<SpriteRendererComponent>("Sprite Renderer", _entity, [](auto& _component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(_component.Color));
			});
	}

}