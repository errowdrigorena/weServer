#!/usr/bin/env python3
"""Manual and CI smoke tests for the weServer CRUD endpoints over HTTPS."""

from __future__ import annotations

import argparse
import json
import ssl
import sys
import urllib.error
import urllib.request

BASE = "https://127.0.0.1:8080/crud/items"
CTX = ssl.create_default_context()
CTX.check_hostname = False
CTX.verify_mode = ssl.CERT_NONE


def request(method: str, url: str, body: dict | None = None) -> tuple[int, str]:
    data = None
    headers = {}
    if body is not None:
        data = json.dumps(body).encode()
        headers["Content-Type"] = "application/json"

    req = urllib.request.Request(url, data=data, headers=headers, method=method)
    try:
        with urllib.request.urlopen(req, context=CTX, timeout=5) as resp:
            payload = resp.read().decode()
            return resp.status, payload
    except urllib.error.HTTPError as err:
        payload = err.read().decode()
        return err.code, payload


def assert_status(label: str, status: int, expected: int, body: str) -> None:
    if status != expected:
        raise AssertionError(
            f"{label}: expected HTTP {expected}, got {status}. Body: {body!r}"
        )


def run_ci_tests() -> int:
    print("CI smoke: GET /crud/items (empty)")
    status, body = request("GET", BASE)
    assert_status("GET list (initial)", status, 200, body)
    items = json.loads(body)
    if items != []:
        raise AssertionError(f"Expected empty list, got {items!r}")

    print("CI smoke: POST /crud/items")
    status, body = request("POST", BASE, {"name": "foo", "description": "bar"})
    assert_status("POST create", status, 200, body)
    item = json.loads(body)
    if item.get("name") != "foo" or item.get("description") != "bar":
        raise AssertionError(f"Unexpected created item: {item!r}")
    item_id = item["id"]

    print(f"CI smoke: GET /crud/items/{item_id}")
    status, body = request("GET", f"{BASE}/{item_id}")
    assert_status("GET item", status, 200, body)
    fetched = json.loads(body)
    if fetched.get("id") != item_id:
        raise AssertionError(f"Unexpected fetched item: {fetched!r}")

    print(f"CI smoke: PUT /crud/items/{item_id}")
    status, body = request(
        "PUT", f"{BASE}/{item_id}", {"name": "baz", "description": "updated"}
    )
    assert_status("PUT update", status, 200, body)
    updated = json.loads(body)
    if updated.get("name") != "baz":
        raise AssertionError(f"Unexpected updated item: {updated!r}")

    print(f"CI smoke: DELETE /crud/items/{item_id}")
    status, body = request("DELETE", f"{BASE}/{item_id}")
    assert_status("DELETE item", status, 204, body)

    print("CI smoke: GET /crud/items (after delete)")
    status, body = request("GET", BASE)
    assert_status("GET list (final)", status, 200, body)
    if json.loads(body) != []:
        raise AssertionError("Expected empty list after delete")

    print("CRUD smoke test passed.")
    return 0


def run_manual_demo() -> int:
    print("GET /crud/items")
    status, body = request("GET", BASE)
    print(status, body)

    print("\nPOST /crud/items")
    status, body = request("POST", BASE, {"name": "foo", "description": "bar"})
    print(status, body)
    item = json.loads(body)
    item_id = item["id"]

    print(f"\nGET /crud/items/{item_id}")
    status, body = request("GET", f"{BASE}/{item_id}")
    print(status, body)

    print(f"\nPUT /crud/items/{item_id}")
    status, body = request(
        "PUT", f"{BASE}/{item_id}", {"name": "baz", "description": "updated"}
    )
    print(status, body)

    print(f"\nDELETE /crud/items/{item_id}")
    status, body = request("DELETE", f"{BASE}/{item_id}")
    print(status, body or "<empty>")

    print("\nGET /crud/items (after delete)")
    status, body = request("GET", BASE)
    print(status, body)
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--ci",
        action="store_true",
        help="Assert expected HTTP responses (for CI smoke tests)",
    )
    args = parser.parse_args()

    if args.ci:
        return run_ci_tests()
    return run_manual_demo()


if __name__ == "__main__":
    sys.exit(main())
