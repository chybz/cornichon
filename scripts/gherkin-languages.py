#!/usr/bin/env python

import sys
import json

GLURL = "https://raw.githubusercontent.com"
GLURL += "/cucumber/gherkin/main/gherkin-languages.json"
INDENT = "    "


def pindent(*args, **kwargs):
    pad = INDENT * (kwargs["count"] if "count" in kwargs else 1)
    sys.stdout.write(''.join([pad, *args]))

    if "newline" in kwargs:
        sys.stdout.write("\n")


if __name__ == '__main__':
    if len(sys.argv) < 2:
        raise RuntimeError("missing json file")

    # Opening JSON file
    f = open(sys.argv[1])

    # returns JSON object as
    # a dictionary
    data = json.load(f)

    print('{')

    firstl = True

    for lang in sorted(data.keys()):
        if not firstl:
            sys.stdout.write(",\n")

        pindent("{", newline=True)
        pindent(f'"{lang}",', newline=True, count=2)
        pindent('{', count=2, newline=True)

        firstk = True

        for k in data[lang].keys():
            if not isinstance(data[lang][k], list):
                continue

            if not firstk:
                sys.stdout.write(",\n")

            pindent(
                '{ ',
                f'"{k}", ',
                '{ ',
                ", ".join(f'"{t.strip()}"' for t in data[lang][k]),
                " }",
                " }",
                count=3
            )

            if firstk:
                firstk = False

        print('')
        pindent("}", count=2, newline=True)
        pindent("}")

        if firstl:
            firstl = False

    print("\n}")
