includes("../hmcd")

target("hmcd-cli")
    set_version("3.0.1")
    set_kind("binary")
    set_configdir(".")
    add_configfiles("hmcd-cli_config.h.in")
    add_files(
        "main.c")
    add_includedirs(ROOT_DIR .. "include")
    add_deps("hmcd_static", "hmcd_shared")
    add_linkdirs(ROOT_DIR .. "build")
    add_links("curl", "hmcd")
    add_installfiles(
        ROOT_DIR .. "LICENSE.md",
        ROOT_DIR .. "README.md", {prefixdir = "share/hmcd"})
    local local_ROOT_DIR = ROOT_DIR
    after_build(function (target)
        os.cp(target:targetfile(), local_ROOT_DIR .. "build")
    end)
target_end()