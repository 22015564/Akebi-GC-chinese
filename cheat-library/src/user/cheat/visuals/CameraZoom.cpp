#include "pch-il2cpp.h"
#include "CameraZoom.h"

#include <helpers.h>
#include <cheat/events.h>

namespace cheat::feature
{
    static void SCameraModuleInitialize_SetWarningLocateRatio_Hook(app::SCameraModuleInitialize* __this, double deltaTime, app::CameraShareData* data, MethodInfo* method);

    CameraZoom::CameraZoom() : Feature(),
        NF(f_Enabled, u8"摄像机距离", "Visuals::CameraZoom", false),
        NF(f_Zoom, u8"距离", "Visuals::CameraZoom", 200)
    {
        HookManager::install(app::MoleMole_SCameraModuleInitialize_SetWarningLocateRatio, SCameraModuleInitialize_SetWarningLocateRatio_Hook);
    }

    const FeatureGUIInfo& CameraZoom::GetGUIInfo() const
    {
        static const FeatureGUIInfo info{ u8"摄像机距离", u8"图像渲染", false };
        return info;
    }

    void CameraZoom::DrawMain()
    {
        ConfigWidget("", f_Enabled); ImGui::SameLine();
        ConfigWidget(u8"摄像机距离", f_Zoom, 0.01f, 1.0f, 500.0f, u8"自定义视角距离.\n"
            u8"自定义的值是默认缩放距离的乘数.\n"
            u8"比如:\n"
            u8"\t2.0 = 2.0 * 默认缩放"
        );
    }

    bool CameraZoom::NeedStatusDraw() const
    {
        return f_Enabled;
    }

    void CameraZoom::DrawStatus()
    {
        ImGui::Text(u8"自定义视距 [%.1fx]", f_Zoom.value());
    }

    CameraZoom& CameraZoom::GetInstance()
    {
        static CameraZoom instance;
        return instance;
    } 

    void SCameraModuleInitialize_SetWarningLocateRatio_Hook(app::SCameraModuleInitialize* __this, double deltaTime, app::CameraShareData* data, MethodInfo* method)
    {
        CameraZoom& cameraZoom = CameraZoom::GetInstance();
        if (cameraZoom.f_Enabled)
        {
            data->currentWarningLocateRatio= static_cast<double>(cameraZoom.f_Zoom);
            //data->isRadiusSqueezing;
        }
        else
            data->currentWarningLocateRatio = 1.0;
        
        CALL_ORIGIN(SCameraModuleInitialize_SetWarningLocateRatio_Hook, __this, deltaTime, data, method);
    }
}

