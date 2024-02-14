includes("../hma")

target("hma-cli")
    set_version("3.0.1")
    set_kind("binary")
    set_configdir(".")
    add_configfiles("hma-cli_config.h.in")
    add_files(
        "main.c")
    add_includedirs(ROOT_DIR .. "include")
    add_deps("hma_static", "hma_shared")
    add_linkdirs(ROOT_DIR .. "build")
    add_links("curl", "hma")
    add_installfiles(
        ROOT_DIR .. "LICENSE.md",
        ROOT_DIR .. "README.md", {prefixdir = "share/hma"})
    local local_ROOT_DIR = ROOT_DIR
    after_build(function (target)
        os.cp(target:targetfile(), local_ROOT_DIR .. "build")
    end)
target_end()