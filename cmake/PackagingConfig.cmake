# =============================================================================
# PackagingConfig.cmake - CPack Configuration for cpp-template Modern CMake packaging configuration
# with support for multiple package formats
#
# This module configures CPack to generate installation packages for the cpp-template project. It
# supports multiple package formats including: - ZIP/TGZ archives for all platforms - DEB packages
# for Debian/Ubuntu - RPM packages for Red Hat/CentOS/Fedora - DMG/PKG packages for macOS - MSI/NSIS
# installers for Windows
# =============================================================================

# Only configure packaging if this is the main project
if(PROJECT_IS_TOP_LEVEL OR CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    message(STATUS "Configuring packaging for ${PROJECT_NAME}")

    # Set package contact before including CPack
    set(CPACK_PACKAGE_CONTACT "developer@example.com")

    # Include CPack module
    include(CPack)

    # ============================================================================= Basic Package
    # Information
    # =============================================================================

    # Use project metadata for package information
    set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
    set(CPACK_PACKAGE_VENDOR "cpp-template project")

    # Package description file (if it exists)
    if(EXISTS "${CMAKE_SOURCE_DIR}/README.md")
        set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
    endif()

    # License file (if it exists)
    if(EXISTS "${CMAKE_SOURCE_DIR}/LICENSE")
        set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
    elseif(EXISTS "${CMAKE_SOURCE_DIR}/LICENSE.txt")
        set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
    endif()

    # Welcome and readme files
    if(EXISTS "${CMAKE_SOURCE_DIR}/INSTALL.md")
        set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/INSTALL.md")
    elseif(EXISTS "${CMAKE_SOURCE_DIR}/README.md")
        set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
    endif()

    # ============================================================================= Package File
    # Naming
    # =============================================================================

    # Create descriptive package names Format: projectname-version-platform-architecture
    if(WIN32)
        set(CPACK_SYSTEM_NAME "windows")
    elseif(APPLE)
        set(CPACK_SYSTEM_NAME "macos")
    elseif(UNIX)
        set(CPACK_SYSTEM_NAME "linux")
    endif()

    # Add architecture to system name
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CPACK_SYSTEM_NAME "${CPACK_SYSTEM_NAME}-x64")
    else()
        set(CPACK_SYSTEM_NAME "${CPACK_SYSTEM_NAME}-x86")
    endif()

    # Add ARM architecture detection for Apple Silicon
    if(APPLE AND CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        set(CPACK_SYSTEM_NAME "macos-arm64")
    endif()

    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME}")

    # ============================================================================= Installation
    # Directory Structure
    # =============================================================================

    # Set installation prefix
    if(WIN32)
        set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}")
    else()
        set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
    endif()

    # ============================================================================= Generator
    # Selection and Configuration
    # =============================================================================

    # Default generators for each platform
    if(WIN32)
        # Windows: ZIP archive and NSIS installer
        list(APPEND CPACK_GENERATOR "ZIP")
        if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.18")
            # NSIS requires CMake 3.18+
            list(APPEND CPACK_GENERATOR "NSIS")
        endif()
    elseif(APPLE)
        # macOS: TGZ archive and DMG disk image
        list(APPEND CPACK_GENERATOR "TGZ")
        list(APPEND CPACK_GENERATOR "DragNDrop")
    elseif(UNIX)
        # Linux: TGZ archive and DEB/RPM packages
        list(APPEND CPACK_GENERATOR "TGZ")

        # Add DEB generator if dpkg-deb is available
        find_program(DPKG_DEB_EXECUTABLE dpkg-deb)
        if(DPKG_DEB_EXECUTABLE)
            list(APPEND CPACK_GENERATOR "DEB")
        endif()

        # Add RPM generator if rpmbuild is available
        find_program(RPMBUILD_EXECUTABLE rpmbuild)
        if(RPMBUILD_EXECUTABLE)
            list(APPEND CPACK_GENERATOR "RPM")
        endif()
    endif()

    # ============================================================================= Archive
    # Generator Configuration
    # =============================================================================

    # ZIP and TGZ archive settings
    set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

    # ============================================================================= DEB Package
    # Configuration (Linux)
    # =============================================================================

    if("DEB" IN_LIST CPACK_GENERATOR)
        # Basic DEB package information
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
        set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
        set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
        set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "${PROJECT_HOMEPAGE_URL}")

        # Automatically determine dependencies
        set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

        # Package relationships
        set(CPACK_DEBIAN_PACKAGE_SUGGESTS "cmake, build-essential")

        # Control file fields
        set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION ON)

        message(STATUS "DEB package generation enabled")
    endif()

    # ============================================================================= RPM Package
    # Configuration (Linux)
    # =============================================================================

    if("RPM" IN_LIST CPACK_GENERATOR)
        # Basic RPM package information
        set(CPACK_RPM_PACKAGE_SUMMARY "${PROJECT_DESCRIPTION}")
        set(CPACK_RPM_PACKAGE_DESCRIPTION "${PROJECT_DESCRIPTION}")
        set(CPACK_RPM_PACKAGE_LICENSE "MIT")
        set(CPACK_RPM_PACKAGE_GROUP "Development/Tools")
        set(CPACK_RPM_PACKAGE_URL "${PROJECT_HOMEPAGE_URL}")

        # Automatically determine dependencies
        set(CPACK_RPM_PACKAGE_AUTOREQ ON)
        set(CPACK_RPM_PACKAGE_AUTOPROV ON)

        # Package relationships
        set(CPACK_RPM_PACKAGE_SUGGESTS "cmake, gcc-c++, make")

        # Exclude debug information from separate package
        set(CPACK_RPM_DEBUGINFO_PACKAGE OFF)

        message(STATUS "RPM package generation enabled")
    endif()

    # ============================================================================= NSIS Installer
    # Configuration (Windows)
    # =============================================================================

    if("NSIS" IN_LIST CPACK_GENERATOR)
        # Installer appearance
        set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_NAME} ${CPACK_PACKAGE_VERSION}")
        set(CPACK_NSIS_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")

        # Installation options
        set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
        set(CPACK_NSIS_MODIFY_PATH ON)

        # Menu and desktop shortcuts
        set(CPACK_NSIS_CREATE_ICONS_EXTRA
            "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${CPACK_PACKAGE_NAME}.lnk' '$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe'"
        )
        set(CPACK_NSIS_DELETE_ICONS_EXTRA
            "Delete '$SMPROGRAMS\\\\$START_MENU\\\\${CPACK_PACKAGE_NAME}.lnk'")

        # Installer/uninstaller icon (if available)
        if(EXISTS "${CMAKE_SOURCE_DIR}/resources/icon.ico")
            set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/resources/icon.ico")
            set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/resources/icon.ico")
        endif()

        message(STATUS "NSIS installer generation enabled")
    endif()

    # ============================================================================= DMG
    # Configuration (macOS)
    # =============================================================================

    if("DragNDrop" IN_LIST CPACK_GENERATOR)
        # DMG appearance
        set(CPACK_DMG_VOLUME_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
        set(CPACK_DMG_FORMAT "UDBZ") # Compressed DMG

        # Background image (if available)
        if(EXISTS "${CMAKE_SOURCE_DIR}/resources/dmg_background.png")
            set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_SOURCE_DIR}/resources/dmg_background.png")
        endif()

        # DMG window properties
        set(CPACK_DMG_DS_STORE_SETUP_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/DMGSetup.scpt")

        message(STATUS "DMG package generation enabled")
    endif()

    # ============================================================================= Component-based
    # Installation
    # =============================================================================

    # Define installation components
    set(CPACK_COMPONENTS_ALL Runtime Development Documentation)

    # Runtime component (executables and shared libraries)
    set(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME "Runtime")
    set(CPACK_COMPONENT_RUNTIME_DESCRIPTION "Executable files and runtime libraries")
    set(CPACK_COMPONENT_RUNTIME_REQUIRED ON)

    # Development component (headers, static libraries, CMake files)
    set(CPACK_COMPONENT_DEVELOPMENT_DISPLAY_NAME "Development")
    set(CPACK_COMPONENT_DEVELOPMENT_DESCRIPTION "Header files and development libraries")
    set(CPACK_COMPONENT_DEVELOPMENT_DEPENDS Runtime)

    # Documentation component
    set(CPACK_COMPONENT_DOCUMENTATION_DISPLAY_NAME "Documentation")
    set(CPACK_COMPONENT_DOCUMENTATION_DESCRIPTION "User documentation and examples")

    # Enable component-based installation for supported generators
    set(CPACK_DEB_COMPONENT_INSTALL ON)
    set(CPACK_RPM_COMPONENT_INSTALL ON)
    set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

    # ============================================================================= Build Summary
    # =============================================================================

    message(STATUS "Package generators: ${CPACK_GENERATOR}")
    message(STATUS "Package name: ${CPACK_PACKAGE_FILE_NAME}")
    message(STATUS "Installation components: ${CPACK_COMPONENTS_ALL}")

    # Display packaging commands
    message(STATUS "To create packages, run:")
    message(STATUS "  cmake --build build --target package")
    message(STATUS "  # Or directly: cpack --config build/CPackConfig.cmake")

else()
    message(STATUS "Packaging disabled - not top-level project")
endif()
