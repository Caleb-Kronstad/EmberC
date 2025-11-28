OutputDir = "%{cfg.buildcfg}"

workspace "emberc"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "ember"
    location "build"

-- EMBER --
project "ember"
    kind "ConsoleApp"
    language "C"
    location "build/build-files"
    
    targetdir ("build/bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("build/bin-int/" .. OutputDir .. "/%{prj.name}")
    
    files {
        "src/**.h",
        "src/**.c",
    }

    defines {
        "CIMGUI_DEFINE_ENUMS_AND_STRUCTS",
        "CIMGUI_USE_GLFW",
        "CIMGUI_USE_OPENGL3"
    }
    
    includedirs {
        "src",
        "dependencies/glad/include",
        "dependencies/glfw/include",
        "dependencies/miniaudio",
        "dependencies/cimgui",
        "dependencies/cjson"
    }
    
    links {
        "glfw",
        "glad",
        "miniaudio",
        "cimgui",
        "cjson"
    }
    
    filter "system:windows"
        systemversion "latest"
        defines { 
            "EMBER_PLATFORM_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS"
        }
        links {
            "opengl32",
            "comdlg32",
            "gdi32"
        }
        
    filter "system:linux"
        pic "On"
        defines { 
            "EMBER_PLATFORM_LINUX" 
        }
        links {
            "GL",
            "dl",
            "pthread",
            "X11",
            "Xrandr",
            "Xi",
            "Xxf86vm",
            "Xcursor",
            "Xinerama"
        }
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"
        
    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "Off"

group "dependencies"

-- MINIAUDIO --
project "miniaudio"
    location "build/build-files"
    kind "StaticLib"
    language "C"

    targetdir ("build/bin/" .. OutputDir .. "/dependencies/%{prj.name}")
    objdir ("build/bin-int/" .. OutputDir .. "/dependencies/%{prj.name}")

    files {
        "dependencies/miniaudio/miniaudio.h",
        "dependencies/miniaudio/miniaudio.c",
    }

    includedirs {
        "dependencies/miniaudio"
    }

    filter "system:windows"
        systemversion "latest"
        defines {
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "system:linux"
        pic "On"

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "Off"

-- CJSON --
project "cjson"
    location "build/build-files"
    kind "StaticLib"
    language "C"

    targetdir ("build/bin/" .. OutputDir .. "/dependencies/%{prj.name}")
    objdir ("build/bin-int/" .. OutputDir .. "/dependencies/%{prj.name}")
    
    files {
        "dependencies/cjson/cJSON.h",
        "dependencies/cjson/cJSON.c"
    }

    includedirs {
        "dependencies/cjson"
    }
    
    defines {
        "CJSON_HIDE_SYMBOLS"
    }
    
    filter "system:windows"
        systemversion "latest"
        defines {
            "_CRT_SECURE_NO_WARNINGS"
        }
    
    filter "system:linux"
        pic "On"
        links {
            "m"
        }
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "Off"   

-- GLAD --
project "glad"
    location "build/build-files"
    kind "StaticLib"
    language "C"

    targetdir ("build/bin/" .. OutputDir .. "/dependencies/%{prj.name}")
    objdir ("build/bin-int/" .. OutputDir .. "/dependencies/%{prj.name}")

    files {
        "dependencies/glad/include/glad/glad.h",
        "dependencies/glad/include/KHR/khrplatform.h",
        "dependencies/glad/src/glad.c"
    }

    includedirs {
        "dependencies/glad/include"
    }

    filter "system:windows"
        systemversion "latest"
        defines {
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "system:linux"
        pic "On"

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "Off"

-- GLFW --
project "glfw"
    location "build/build-files"
    kind "StaticLib"
    language "C"

    targetdir ("build/bin/" .. OutputDir .. "/dependencies/%{prj.name}")
    objdir ("build/bin-int/" .. OutputDir .. "/dependencies/%{prj.name}")

    files {
        "dependencies/glfw/include/GLFW/glfw3.h",
        "dependencies/glfw/include/GLFW/glfw3native.h",
        "dependencies/glfw/src/glfw_config.h",
        "dependencies/glfw/src/context.c",
        "dependencies/glfw/src/init.c",
        "dependencies/glfw/src/input.c",
        "dependencies/glfw/src/monitor.c",
        "dependencies/glfw/src/vulkan.c",
        "dependencies/glfw/src/window.c",
        "dependencies/glfw/src/egl_context.c",
        "dependencies/glfw/src/osmesa_context.c"
    }

    includedirs {
        "dependencies/glfw/include"
    }

    filter "system:windows"
        systemversion "latest"

        files {
            "dependencies/glfw/src/win32_init.c",
            "dependencies/glfw/src/win32_joystick.c",
            "dependencies/glfw/src/win32_monitor.c",
            "dependencies/glfw/src/win32_time.c",
            "dependencies/glfw/src/win32_thread.c",
            "dependencies/glfw/src/win32_window.c",
            "dependencies/glfw/src/wgl_context.c"
        }

        defines {
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "system:linux"
        pic "On"

        files {
            "dependencies/glfw/src/x11_init.c",
            "dependencies/glfw/src/x11_monitor.c",
            "dependencies/glfw/src/x11_window.c",
            "dependencies/glfw/src/xkb_unicode.c",
            "dependencies/glfw/src/posix_time.c",
            "dependencies/glfw/src/posix_thread.c",
            "dependencies/glfw/src/glx_context.c",
            "dependencies/glfw/src/linux_joystick.c"
        }

        defines {
            "_GLFW_X11"
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "Off"

-- CIMGUI --
project "cimgui"
    location "build/build-files"
    kind "StaticLib"
    language "C++"
    cppdialect "C++11"
    

    targetdir ("build/bin/" .. OutputDir .. "/dependencies/%{prj.name}")
    objdir ("build/bin-int/" .. OutputDir .. "/dependencies/%{prj.name}")
    
    files {
        "dependencies/cimgui/cimgui.cpp",
        "dependencies/cimgui/imgui/imgui.cpp",
        "dependencies/cimgui/imgui/imgui_draw.cpp",
        "dependencies/cimgui/imgui/imgui_demo.cpp",
        "dependencies/cimgui/imgui/imgui_widgets.cpp",
        "dependencies/cimgui/imgui/imgui_tables.cpp",
        "dependencies/cimgui/imgui/backends/imgui_impl_glfw.cpp",
        "dependencies/cimgui/imgui/backends/imgui_impl_opengl3.cpp"
    }
    
    includedirs {
        "dependencies/cimgui",
        "dependencies/cimgui/imgui",
        "dependencies/cimgui/imgui/backends",
        "dependencies/glfw/include"
    }
    
    defines {
        "IMGUI_USER_CONFIG=\"../cimconfig.h\"",
        "IMGUI_DISABLE_OBSOLETE_FUNCTIONS=1"
    }
    
    filter "system:windows"
        systemversion "latest"
        defines {
            "IMGUI_IMPL_API=extern\t\"C\"\t__declspec(dllexport)"
        }
        links {
            "imm32"
        }
    
    filter "system:linux"
        pic "On"
        defines {
            "IMGUI_IMPL_API=extern\t\"C\"\t"
        }
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "Off"

group ""