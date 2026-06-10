set_project("calculator")
set_version("1.0.0")

set_languages("c11")

add_rules("mode.debug", "mode.release")

-- clang compile commands
add_rules("plugin.compile_commands.autoupdate")

-- =========================================================
-- Options
-- =========================================================

option("tests")
set_default(false)
set_showmenu(true)
option_end()

option("asan")
set_default(false)
set_showmenu(true)
option_end()

-- =========================================================
-- Packages
-- =========================================================

-- Run xmake repo --add lae-repo https://laentropia-homelab.tail7368da.ts.net/laentropia/xmake-repo.git
-- to add my repo :)

add_requires("lae_arena")
add_requires("lae_arraylist")
add_requires("cmocka", { optional = true })

-- =========================================================
-- Library
-- =========================================================

target("calculator_lib")
set_kind("static")

add_packages("lae_arena", "lae_arraylist", {
	public = true,
})

add_files("src/lexer.c", "src/parser.c", "src/evaluator.c")

add_headerfiles("include/*.h")

add_includedirs("include", {
	public = true,
})

if is_mode("debug") then
	add_cflags("-Wall", "-Wextra", "-Wpedantic")
end

if not is_plat("windows") then
	add_syslinks("m")
end

-- =========================================================
-- Executable
-- =========================================================

target("calculator")
set_kind("binary")

add_deps("calculator_lib")

add_files("src/main.c")

-- =========================================================
-- Tests
-- =========================================================

if has_config("tests") then
	local test_files = {
		"test_lexer",
		"test_parser",
		"test_evaluator",
	}

	for _, name in ipairs(test_files) do
		target(name)
		set_kind("binary")

		add_files("test/" .. name .. ".c")

		add_deps("calculator_lib")

		add_packages("cmocka")

		add_tests("default")

		if has_config("asan") then
			add_cflags("-fsanitize=address", "-fno-omit-frame-pointer", {
				force = true,
			})

			add_ldflags("-fsanitize=address", {
				force = true,
			})
		end
	end
end
