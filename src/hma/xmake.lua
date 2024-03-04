target("hma_static")
    set_version("3.1.0")
    set_kind("static")
    set_basename("hma")
    set_configdir(ROOT_DIR .. "include/hma")
    add_configfiles(ROOT_DIR .. "include/hma/hma_config.h.in")
    add_files(
        "hma.c")
    add_includedirs(ROOT_DIR .. "include")
    add_links("curl")
    add_headerfiles(
        ROOT_DIR .. "include/(hma/hma_config.h)",
        ROOT_DIR .. "include/(hma/hma.h)")
    local local_ROOT_DIR = ROOT_DIR
    after_build(function (target)
        os.cp(target:targetfile(), local_ROOT_DIR .. "build")
    end)
target_end()

target("hma_shared")
    set_version("3.1.0")
    set_kind("shared")
    set_basename("hma")
    set_configdir(ROOT_DIR .. "include")
    add_configfiles(ROOT_DIR .. "include" .. "/" .. "hma_config.h.in")
    add_files(
        "hma.c")
    add_includedirs(ROOT_DIR .. "include")
    add_links("curl")
    local local_ROOT_DIR = ROOT_DIR
    after_build(function (target)
        os.cp(target:targetfile(), local_ROOT_DIR .. "build")
    end)
target_end()