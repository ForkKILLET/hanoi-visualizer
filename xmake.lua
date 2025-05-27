add_requires("raylib", {system = true})

target("hanoi")
    set_kind("binary")
    add_cxflags("-std=c++23")
    if is_mode("debug") then
        add_cxflags("-g")
    end
    add_includedirs("inc")
    add_links("raylib")
    add_files("src/*.cpp")
    add_files("src/**/*.cpp")