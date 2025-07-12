# SkyrimSE-Plugin-Template

This is a custom CMake setup designed specifically for Skyrim Special Edition SKSE plugin development. The setup provides a streamlined and flexible build environment to help modders and developers quickly configure, build, and maintain their Skyrim SE plugins.

When creating a new project, you can select from three templates to suit your needs:

- **Bare:** Includes the essential files and configurations to get you up and running quickly.
- **Basic:** Adds a console hook and event registration to help you start integrating with Skyrim’s runtime events.
- **Menu:** Builds on the Basic template by including an ImGui menu system for in-game user interfaces.

This setup supports selecting different CommonLibSSE forks (**PO3** or **NG**) during project initialization, ensuring compatibility with your target Skyrim version and modding preferences.

---

## Build Requirements
- [Git - Distributed Version Control System](https://git-scm.com/downloads/win)
- [CMake - Powerful Software Build System (version 3.24+)](https://cmake.org/download/)
- [Visual Studio 2022 IDE - Programming Tool for Software Developers](https://visualstudio.microsoft.com/vs/)

Make sure to install the following workloads/components in Visual Studio 2022:
- Desktop development with C++
- MSVC v143 - VS 2022 C++ x64/x86 build tools (Latest)
- Windows 10 SDK (10.0.20348.0)

---

## Getting Started

1. **Download the template:**  
   Click the green **Code** button at the top of the repository page and select **Download ZIP**.  
   Extract the ZIP archive to a folder on your computer.

2. **Review the documentation:**  
   Inside the extracted folder, open the `cmake/documentation/readme.html` file in your web browser.  
   This comprehensive documentation will guide you through configuring and building your plugin project.

3. **Run the bootstrap script:**  
   Launch the `CMake_Build.bat` script from the root folder. This will guide you through project setup options like selecting your CommonLibSSE fork, setting project metadata, and choosing a project template.

4. **Open the generated solution:**  
   After the bootstrap completes, open the generated Visual Studio solution located at:  
   `build/vs2022-[po3|ng]/<YourProjectName>.sln`

5. **Build and develop your plugin:**  
   Use Visual Studio to build your plugin and start development.

---