target("hmcd_static")
    set_version("3.0.1")
    set_kind("static")
    set_basename("hmcd")
    set_configdir(ROOT_DIR .. "include/hmcd")
    add_configfiles(ROOT_DIR .. "include/hmcd/hmcd_config.h.in")
    add_files(
        "hmcd.c")
    add_includedirs(ROOT_DIR .. "include")
    add_links("curl")
    add_headerfiles(
        ROOT_DIR .. "include/(hmcd/hmcd_config.h)",
        ROOT_DIR .. "include/(hmcd/hmcd.h)")
    local local_ROOT_DIR = ROOT_DIR
    after_build(function (target)
        os.cp(target:targetfile(), local_ROOT_DIR .. "build")
    end)
target_end()

target("hmcd_shared")
    set_version("3.0.1")
    set_kind("shared")
    set_basename("hmcd")
    set_configdir(ROOT_DIR .. "include")
    add_configfiles(ROOT_DIR .. "include" .. "/" .. "hmcd_config.h.in")
    add_files(
        "hmcd.c")
    add_includedirs(ROOT_DIR .. "include")
    add_links("curl")
    local local_ROOT_DIR = ROOT_DIR
    after_build(function (target)
        os.cp(target:targetfile(), local_ROOT_DIR .. "build")
    end)
target_end()