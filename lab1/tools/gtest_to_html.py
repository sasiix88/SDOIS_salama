#!/usr/bin/env python3
"""Runs GoogleTest executables and writes their results as one HTML page.

Usage: gtest_to_html.py --output report.html test_executable [test_executable ...]

The page is written even when some tests fail. The exit code is 0 when every
test passed and 1 otherwise, so CMake and CI can still detect failures.
"""
import argparse
import datetime
import html
import subprocess
import sys
import tempfile
import xml.etree.ElementTree as ElementTree
from pathlib import Path

STYLE = """
body { font-family: Arial, sans-serif; margin: 2em auto; max-width: 960px; color: #222; }
h1 { margin-bottom: 0.2em; }
.summary { padding: 0.8em 1em; border-radius: 6px; font-size: 1.1em; margin: 1em 0; }
.ok { background: #e3f6e5; border: 1px solid #5cb85c; }
.bad { background: #fde8e8; border: 1px solid #d9534f; }
table { border-collapse: collapse; width: 100%; margin-bottom: 1.5em; }
th, td { border: 1px solid #ccc; padding: 6px 10px; text-align: left; vertical-align: top; }
th { background: #f0f0f0; }
td.passed { color: #2e7d32; font-weight: bold; }
td.failed { color: #c62828; font-weight: bold; }
td.skipped { color: #8a6d00; font-weight: bold; }
pre { margin: 0; white-space: pre-wrap; }
.meta { color: #666; font-size: 0.9em; }
"""


def run_executable(executable, xml_path):
    """Runs one test executable and returns its exit code."""
    command = [executable, "--gtest_output=xml:" + str(xml_path)]
    return subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True).returncode


def classify(testcase):
    """Returns (status, details) for one <testcase> element."""
    failures = testcase.findall("failure")
    if failures:
        details = "\n\n".join((item.text or item.get("message", "")) for item in failures)
        return "failed", details
    if testcase.find("skipped") is not None or testcase.get("result") in ("skipped", "suppressed"):
        return "skipped", ""
    return "passed", ""


def collect(xml_path):
    """Reads a GoogleTest XML file and returns a list of (suite, test, status, seconds, details)."""
    rows = []
    for testcase in ElementTree.parse(xml_path).getroot().iter("testcase"):
        status, details = classify(testcase)
        rows.append((testcase.get("classname", ""), testcase.get("name", ""), status,
                     testcase.get("time", "0"), details))
    return rows


def render_rows(rows):
    lines = []
    for suite, name, status, seconds, details in rows:
        lines.append(
            "<tr><td>{}</td><td>{}</td><td class='{}'>{}</td><td>{}</td><td><pre>{}</pre></td></tr>".format(
                html.escape(suite), html.escape(name), status, status.upper(), html.escape(seconds),
                html.escape(details)))
    return "\n".join(lines)


def render_page(results):
    """results: list of (executable name, rows, exit code)."""
    all_rows = [row for _, rows, _ in results for row in rows]
    total = len(all_rows)
    failed = sum(1 for row in all_rows if row[2] == "failed")
    skipped = sum(1 for row in all_rows if row[2] == "skipped")
    passed = total - failed - skipped
    crashed = [name for name, rows, code in results if code != 0 and not any(r[2] == "failed" for r in rows)]
    verdict_ok = failed == 0 and not crashed
    parts = ["<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><title>Lab 1 test report</title>",
             "<style>" + STYLE + "</style></head><body>", "<h1>Lab 1 &mdash; unit test report</h1>",
             "<p class='meta'>Generated {}</p>".format(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")),
             "<div class='summary {}'>{} &mdash; {} tests: {} passed, {} failed, {} skipped</div>".format(
                 "ok" if verdict_ok else "bad", "ALL TESTS PASSED" if verdict_ok else "TESTS FAILED",
                 total, passed, failed, skipped)]
    for name in crashed:
        parts.append("<p class='bad summary'>{} exited with an error before reporting all results.</p>".format(
            html.escape(name)))
    for name, rows, _ in results:
        parts.append("<h2>{}</h2>".format(html.escape(name)))
        parts.append("<table><tr><th>Suite</th><th>Test</th><th>Result</th><th>Time, s</th><th>Details</th></tr>")
        parts.append(render_rows(rows))
        parts.append("</table>")
    parts.append("</body></html>")
    return "\n".join(parts), verdict_ok


def main():
    parser = argparse.ArgumentParser(description="Run GoogleTest executables and write an HTML report.")
    parser.add_argument("--output", required=True, help="path of the HTML file to write")
    parser.add_argument("executables", nargs="+", help="GoogleTest executables to run")
    arguments = parser.parse_args()

    results = []
    with tempfile.TemporaryDirectory() as folder:
        for executable in arguments.executables:
            xml_path = Path(folder) / (Path(executable).stem + ".xml")
            code = run_executable(executable, xml_path)
            rows = collect(xml_path) if xml_path.exists() else []
            results.append((Path(executable).stem, rows, code))

    page, verdict_ok = render_page(results)
    Path(arguments.output).parent.mkdir(parents=True, exist_ok=True)
    Path(arguments.output).write_text(page, encoding="utf-8")
    print("Report written to", arguments.output)
    return 0 if verdict_ok else 1


if __name__ == "__main__":
    sys.exit(main())