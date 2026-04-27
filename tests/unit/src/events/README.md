# We use a fake pointer pattern in tests

- Tests create fake pointers by casting integers, e.g. reinterpret_cast<T*>(0x1234).
- These pointers are used only for identity comparisons, never dereferenced.
- Real object construction is avoided to keep tests simple.
- Treat reinterpret_cast<T*>(0) as nullptr.
- Prefer nullptr directly when testing null behavior.
- Always use non-zero values (e.g. 0x1000) for valid fake pointers.
- Use UINTPTR_MAX for edge “max address” cases.
- Never access members or memory through these pointers.
- If behavior needs real objects, create a minimal instance.
