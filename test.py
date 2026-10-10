import sys
import pexpect
from pathlib import Path

BASE = Path(__file__).resolve().parent
EXE = str(BASE / "build" / "main")
print(EXE)
IN_FILE = BASE / "inout-sujet" / "in.txt"
OUT_FILE = BASE / "inout-sujet" / "out.txt"
TIMEOUT = 5 
RESPONSE_TIMEOUT = 0.2

GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
RESET = "\033[0m"

inputs = IN_FILE.read_text().splitlines()
expected = OUT_FILE.read_text().splitlines()


def main():
    child = pexpect.spawn(EXE, encoding="utf-8", timeout=TIMEOUT, cwd=str(BASE))
    child.setecho(False)  # évite de relire sa propre saisie

    received = []
    for i, line in enumerate(inputs):
        print(f"[{i}] Envoi : {line!r}")
        child.sendline(line)

        response = []
        if line.strip() == "EXIT":
            try:
                child.expect(pexpect.EOF)
                response.extend(child.before.splitlines())
            except pexpect.TIMEOUT:
                print(f"    TIMEOUT : le programme ne s'est pas terminé en {TIMEOUT}s")
                print(f"    Tampon reçu : {child.before!r}")
                child.close(force=True)
                sys.exit(1)
        else:
            while True:
                try:
                    child.expect(r"\r?\n", timeout=RESPONSE_TIMEOUT)
                    response.append(child.before)
                except pexpect.TIMEOUT:
                    break
                except pexpect.EOF:
                    response.extend(child.before.splitlines())
                    break

        received.extend(response)
        first_result_index = len(received) - len(response)
        for response_index, result in enumerate(response):
            result_index = first_result_index + response_index
            want = expected[result_index] if result_index < len(expected) else None
            if result == want:
                status = f"{GREEN}CORRECT{RESET}"
            else:
                status = f"{RED}INCORRECT{RESET}"
            print(f"    Réponse : {result!r} -> {status}")
            if result != want:
                print(f"    Attendu : {want!r}")

    child.close()
    ok = sum(got == want for got, want in zip(received, expected))
    max_lines = max(len(received), len(expected))
    print(f"\n{ok}/{max_lines} lignes correctes")
    if len(received) < len(expected):
        missing = len(expected) - len(received)
        print(f"{YELLOW}{missing} ligne(s) attendue(s) manquante(s){RESET}")
    elif len(received) > len(expected):
        extra = len(received) - len(expected)
        print(f"{YELLOW}{extra} ligne(s) supplémentaire(s) reçue(s){RESET}")
    sys.exit(0 if received == expected else 1)


if __name__ == "__main__":
    main()




