import sys
import xml.etree.ElementTree as ET
from pathlib import Path


def test_summary(path):
    print("## Tests")
    if not path.is_file():
        print("No JUnit report was produced; see the build/test log.")
        return
    root = ET.parse(path).getroot()
    cases = list(root.iter("testcase"))
    failed = sum(case.find("failure") is not None or case.find("error") is not None for case in cases)
    skipped = sum(case.find("skipped") is not None for case in cases)
    passed = len(cases) - failed - skipped
    print("| Passed | Failed | Skipped | Total |")
    print("| ---: | ---: | ---: | ---: |")
    print(f"| {passed} | {failed} | {skipped} | {len(cases)} |")
    print("\nCounts represent CTest checks, including the Catch2 suites and CLI smoke test.")


def coverage_summary(path):
    print("\n## Source Library Coverage (gcov)")
    if not path.is_file():
        print("No coverage report was produced; see the coverage log.")
        return
    root = ET.parse(path).getroot()
    print("| Metric | Covered | Total | Coverage |")
    print("| --- | ---: | ---: | ---: |")
    for label, prefix in (("Lines", "lines"), ("Branches", "branches")):
        covered = int(root.attrib[f"{prefix}-covered"])
        total = int(root.attrib[f"{prefix}-valid"])
        rate = f"{100 * covered / total:.2f}%" if total else "N/A"
        print(f"| {label} | {covered} | {total} | {rate} |")
    print("\nScope: `src/` only. Download the HTML report for per-file details.")


def main():
    if len(sys.argv) not in (2, 3):
        raise SystemExit("Usage: report_summary.py JUNIT_XML [COBERTURA_XML]")
    test_summary(Path(sys.argv[1]))
    if len(sys.argv) == 3:
        coverage_summary(Path(sys.argv[2]))


if __name__ == "__main__":
    main()