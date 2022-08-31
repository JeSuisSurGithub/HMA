includes("../hmcdcore")

target("hmcd-cli")
    set_version("3.0.0")
    set_kind("binary")
    set_configdir(".")
    add_configfiles("hmcd-cli_config.h.in")
    add_files(
        "main.c")
    add_includedirs(ROOT_DIR .. "include")
    add_deps("hmcdcore_static", "hmcdcore_shared")
    add_linkdirs(ROOT_DIR .. "build")
    add_links("curl", "hmcdcore")
    add_installfiles(
        ROOT_DIR .. "LICENSE.md",
        ROOT_DIR .. "README.md", {prefixdir = "share/hmcd"})
    local local_ROOT_DIR = ROOT_DIR
    after_build(function (target)
        os.cp(target:targetfile(), local_ROOT_DIR .. "build")
    end)
target_end()