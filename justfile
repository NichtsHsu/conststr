srcs := "./tests/*.cpp"
outpath := "./build"
include := "./include"
default_cc := "/usr/bin/env g++"
cppflags := "-std=c++20 -Wall"
set windows-shell := ["powershell.exe", "-NoLogo", "-Command"]

alias b := build-tests
alias t := run-tests
alias nt := nmake-tests
alias c := clean

build-tests cc=default_cc:
    #!/bin/bash
    set -e
    set -x
    mkdir -p "{{ outpath }}"
    for src in {{ srcs }}; do
        file=`basename $src`;
        outbin="{{ outpath }}/${file%.*}";
        {{ cc }} $src -I"{{ include }}" -o "$outbin" {{ cppflags }};
    done

run-tests cc=default_cc: (build-tests cc)
    #!/bin/bash
    set -e
    set -x
    for bin in "{{ outpath }}"/*; do
        $bin;
    done

nmake-tests:
    nmake -f nmakefile
    Get-ChildItem "{{ outpath }}" -Filter *.exe | Foreach-Object { & $_.FullName }

clean:
    rm -r "{{ outpath }}"
