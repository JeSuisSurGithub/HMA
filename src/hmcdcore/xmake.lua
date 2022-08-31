target("hmcdcore_static")
    set_version("3.0.0")
    set_kind("static")
    set_basename("hmcdcore")
    set_configdir(ROOT_DIR .. "include/hmcdcore")
    add_configfiles(ROOT_DIR .. "include/hmcdcore/hmcdcore_config.h.in")
    add_files(
        "hmcd.c")
    add_includedirs(ROOT_DIR .. "include")
    add_links("curl")
    add_headerfiles(
        ROOT_DIR .. "include/(hmcdcore/hmcdcore_config.h)",
        ROOT_DIR .. "include/(hmcdcore/hmcd.h)")
    local local_ROOT_DIR = ROOT_DIR
    after_build(function (target)
        os.cp(target:targetfile(), local_ROOT_DIR .. "build")
    end)
target_end()

target("hmcdcore_shared")
    set_version("3.0.0")
    set_kind("shared")
    set_basename("hmcdcore")
    set_configdir(ROOT_DIR .. "include")
    add_configfiles(ROOT_DIR .. "include" .. "/" .. "hmcdcore_config.h.in")
    add_files(
        "hmcd.c")
    add_includedirs(ROOT_DIR .. "include")
    add_links("curl")
    local local_ROOT_DIR = ROOT_DIR
    after_build(function (target)
        os.cp(target:targetfile(), local_ROOT_DIR .. "build")
    end)
target_end()