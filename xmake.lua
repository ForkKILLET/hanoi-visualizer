add_requires("raylib")

target("hanoi")
    set_kind("binary")
    set_languages("c++23")

    if is_mode("debug") then
        add_defines("DEBUG")
        set_symbols("debug")
    elseif is_mode("release") then
        set_symbols("hidden")
        set_strip("all")
        set_optimize("fastest")
    end

    if is_plat("windows") then
        add_ldflags("/SUBSYSTEM:WINDOWS", "/ENTRY:mainCRTStartup", {force = true})
    end

    add_includedirs("inc")
    add_files("src/*.cpp")
    add_files("src/**/*.cpp")

    add_packages("raylib")