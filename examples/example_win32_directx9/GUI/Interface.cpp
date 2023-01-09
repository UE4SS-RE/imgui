#include "Interface.h"
#include <sstream>
#include "imgui.h"
#include "imgui_internal.h"
#include "SodiumLib.h"

Interface* Interface::ifx = nullptr;

Interface* Interface::GetInterface()
{
    if (!ifx)
    {
        ifx = new Interface();
    }
    return ifx;
}

void Interface::DrawInterface()
{
    ImGui::Begin("fuck");
    ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
    ImGuiID dockspaceID = ImGui::GetID("dockspace");
    if (!ImGui::DockBuilderGetNode(dockspaceID))
    {
        ImGui::DockBuilderRemoveNode(dockspaceID);
        ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_None);

        ImGuiID dock_main_id = dockspaceID;
        ImGuiID dock_up_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.05f, nullptr, &dock_main_id);
        ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
        ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
        ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.2f, nullptr, &dock_main_id);
        ImGuiID dock_down_right_id = ImGui::DockBuilderSplitNode(dock_down_id, ImGuiDir_Right, 0.6f, nullptr,
                                                                 &dock_down_id);

        ImGui::DockBuilderDockWindow("Actions", dock_up_id);
        ImGui::DockBuilderDockWindow("Hierarchy", dock_right_id);
        ImGui::DockBuilderDockWindow("Inspector", dock_left_id);
        ImGui::DockBuilderDockWindow("Console", dock_down_id);
        ImGui::DockBuilderDockWindow("Project", dock_down_right_id);
        ImGui::DockBuilderDockWindow("Scene", dock_main_id);

        // Disable tab bar for custom toolbar
        ImGuiDockNode* node = ImGui::DockBuilderGetNode(dock_up_id);
        node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

        ImGui::DockBuilderFinish(dock_main_id);
    }
    ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);

    ImGui::End();
    ImGui::Begin("test");
    ImGui::InputText("TestText", test, IM_ARRAYSIZE(test), 0);
    if (ImGui::Button("test"))
    {
        printf("%s\n", test);
    }
    if (ImGui::Button("Sodium Encrypt"))
    {
        if (SodiumLib::GetSodium()->encryptMessage(cipher, reinterpret_cast<unsigned char*>(test), sizeof test,
                                                   SodiumLib::GetSodium()->internal_nonce,
                                                   SodiumLib::GetSodium()->internal_secret_key))
        {
            printf("Encryption Success!\n");
        }
        else
        {
            printf("Encryption Failed!\n");
        }
#ifdef _DEBUG
        printf("Encrypted Text as hex:\n%s\n", SodiumLib::GetSodium()->writeAsHex(cipher, sizeof cipher).c_str());
#endif
    }
    if (ImGui::Button("Sodium Decrypt"))
    {
        if (SodiumLib::GetSodium()->decryptMessage(decrypted, cipher, sizeof cipher,
                                                   SodiumLib::GetSodium()->internal_nonce,
                                                   SodiumLib::GetSodium()->internal_secret_key))
        {
#ifdef _DEBUG
            std::string ts(std::begin(decrypted), std::end(decrypted));
            printf("Decryption Success!\n%s\n", ts.data());
#endif
        }
    }
    if (ImGui::Button("HexToChar Test Decrypt"))
    {
        auto test = SodiumLib::GetSodium()->writeAsHex(cipher, sizeof cipher);
        auto temp = SodiumLib::GetSodium()->readHex(test);
        auto code = crypto_secretbox_open_easy(decrypted, temp, sizeof temp, SodiumLib::GetSodium()->internal_nonce, SodiumLib::GetSodium()->internal_secret_key);
        if(code != 0)
        {
            printf("F\n");
        }
    }
    ImGui::End();
}

void Interface::SetupFonts()
{
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.Fonts->Clear();
    ImFontConfig font_cfg;
    // NOLINT(clang-diagnostic-array-bounds, clang-diagnostic-pointer-to-int-cast)
    font_cfg.FontDataOwnedByAtlas = false; // if true it will try to free memory and fail
    io.Fonts->AddFontFromMemoryTTF(Roboto, sizeof(Roboto), 14, &font_cfg);
}
