set_xmakever("2.2.5")

set_project("HMA")
set_version("3.0.1")
set_languages("c11")

add_requires("curl", {configs = {binaryonly = true}})

set_warnings("everything")
if is_mode("release") then
    add_cflags("-O3", "-DNDEBUG")
elseif is_mode("debug") then
    add_cflags("-Og", "-g3")
end

if is_plat("window") then
    ROOT_DIR = path.absolute(".") .. "\\"
else
    ROOT_DIR = path.absolute(".") .. "/"
end
includes("src/hma-cli")