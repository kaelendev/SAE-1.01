import sys
import pexpect
from pathlib import Path

BASE = Path(__file__).resolve().parent
EXE = str(BASE / "build" / "main")
print(EXE)
IN_FILE = BASE / "inout-sujet" / "in.txt"
OUT_FILE = BASE / "inout-sujet" / "out.txt"
TIMEOUT = 5 

inputs = IN_FILE.read_text().splitlines()
expected = OUT_FILE.read_text().splitlines()


def main():
    child = pexpect.spawn(EXE, encoding="utf-8", timeout=TIMEOUT, cwd=str(BASE))
    child.setecho(False)  # évite de relire sa propre saisie

    ok = 0
    for i, line in enumerate(inputs):
        print(f"[{i}] Envoi : {line!r}")
        child.sendline(line)

        try:
            child.expect(r"\r?\n")  # attend une ligne complète
        except pexpect.TIMEOUT:
            print(f"    TIMEOUT : aucune réponse en {TIMEOUT}s")
            print(f"    Tampon reçu : {child.before!r}")
            break
        except pexpect.EOF:
            print("    Le programme s'est terminé.")
            print(f"    Sortie restante : {child.before!r}")
            break

        got = child.before.strip("\r\n")
        want = expected[i] if i < len(expected) else None
        status = "OK " if got == want else "DIFF"
        if got == want:
            ok += 1
        print(f"    Reçu    : {got!r}")
        print(f"    Attendu : {want!r}  -> {status}")

    child.close()
    print(f"\n{ok}/{len(inputs)} lignes correctes")
    sys.exit(0 if ok == len(inputs) else 1)


if __name__ == "__main__":
    main()




