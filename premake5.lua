workspace "catplusplus"
  configurations { "Debug", "Release" }
  architecture "x64"
  startproject "catpp"
  source_files = {
    "src/**.h", 
    "src/**.hpp",
    "src/**.cpp",
  }
  
  include "vendor/fmt/premake.lua"

project "catpp"
  kind "ConsoleApp"
  language "C++"
  targetdir "build/bin/%{cfg.buildcfg}"
  objdir "build/obj/%{cfg.buildcfg}"

  files { source_files }

  includedirs {
    "src/",
    "vendor/fmt/include",
    "vendor/CLI11/include"
  }

  libdirs { "vendor" }

  links {
    "fmt"
  }
  
  filter "system:windows"
    cppdialect "C++20"

  filter "system:linux"
    cppdialect "gnu++2a"


  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:Release"
    defines { "RELEASE" }
    optimize "On"
