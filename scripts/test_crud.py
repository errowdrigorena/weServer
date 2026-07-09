#!/usr/bin/env python3
"""Quick manual test for the weServer CRUD endpoints over HTTPS."""

from __future__ import annotations

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


def main() -> int:
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


if __name__ == "__main__":
    sys.exit(main())
