import sys
from pathlib import Path

import pexpect


BASE = Path(__file__).resolve().parent
EXE = str(BASE.parent / "build" / "main")
TIMEOUT = 5
# Délai pour laisser le temps au programme d'écrire toutes les lignes d'une réponse.
RESPONSE_TIMEOUT = 0.2

GREEN = "\033[92m"
RED = "\033[91m"
YELLOW = "\033[93m"
RESET = "\033[0m"


def test_file_pair(
    in_file: Path,
    out_file: Path,
    error_limit: int | None,
    errors: int,
) -> tuple[bool, int, bool]:
    print(f"\nTest : {in_file.parent.name}/{in_file.name}")
    inputs = in_file.read_text().splitlines()
    expected = out_file.read_text().splitlines()

    child = pexpect.spawn(EXE, encoding="utf-8", timeout=TIMEOUT, cwd=str(BASE))
    child.setecho(False)

    received = []
    for i, line in enumerate(inputs):
        print(f"[{i}] Envoi : {line!r}")
        child.sendline(line)

        response = []
        if line.strip().upper() == "EXIT":
            try:
                child.expect(pexpect.EOF)
                response.extend(child.before.splitlines())
            except pexpect.TIMEOUT:
                print(f"    TIMEOUT : le programme ne s'est pas terminé en {TIMEOUT}s")
                print(f"    Tampon reçu : {child.before!r}")
                child.close(force=True)
                errors += 1
                return False, errors, error_limit is not None and errors >= error_limit
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
            status = GREEN + "CORRECT" + RESET if result == want else RED + "INCORRECT" + RESET
            print(f"    Réponse : {result!r} -> {status}")
            if result != want:
                errors += 1
                print(f"    Attendu : {want!r}")
                if error_limit is not None and errors >= error_limit:
                    print(f"{YELLOW}Limite de {error_limit} erreur(s) atteinte.{RESET}")
                    child.close(force=True)
                    return False, errors, True

    child.close()
    ok = sum(got == want for got, want in zip(received, expected))
    max_lines = max(len(received), len(expected))
    print(f"\n{ok}/{max_lines} lignes correctes")
    if len(received) < len(expected):
        missing = len(expected) - len(received)
        errors += missing
        print(f"{YELLOW}{missing} ligne(s) attendue(s) manquante(s){RESET}")
    elif len(received) > len(expected):
        extra = len(received) - len(expected)
        errors += extra
        print(f"{YELLOW}{extra} ligne(s) supplémentaire(s) reçue(s){RESET}")
    stopped = error_limit is not None and errors >= error_limit
    if stopped:
        print(f"{YELLOW}Limite de {error_limit} erreur(s) atteinte.{RESET}")
    return received == expected, errors, stopped


def sprint_tests(sprint: int) -> list[tuple[Path, Path]]:
    sprint_directory = BASE / "inout" / "inout"
    if not sprint_directory.is_dir():
        sprint_directory = BASE / "inout"

    tests = []
    for in_file in sorted(sprint_directory.glob(f"in-sp{sprint}-*.txt")):
        out_file = in_file.with_name(in_file.name.replace("in-", "out-", 1))
        if not out_file.is_file():
            print(f"{RED}Fichier attendu introuvable : {out_file}{RESET}")
            continue
        tests.append((in_file, out_file))
    return tests


def choose_tests() -> list[tuple[Path, Path]]:
    print("Que voulez-vous tester ?")
    print("1. Tous les sprints")
    print("2. Un sprint")
    print("3. Reprendre à partir d'un sprint")
    print("4. Un dossier particulier")
    print("5. Le sujet de base")

    choice = input("Votre choix [5] : ").strip() or "5"

    if choice == "1":
        return [
            test
            for sprint in range(1, 5)
            for test in sprint_tests(sprint)
        ]

    if choice == "2":
        sprint = input("Numéro du sprint (1 à 4) : ").strip()
        if sprint not in {"1", "2", "3", "4"}:
            raise ValueError("Le numéro du sprint doit être compris entre 1 et 4.")
        return sprint_tests(int(sprint))

    if choice == "3":
        sprint = input("Reprendre à partir du sprint (1 à 4) : ").strip()
        if sprint not in {"1", "2", "3", "4"}:
            raise ValueError("Le numéro du sprint doit être compris entre 1 et 4.")
        return [
            test
            for sprint_number in range(int(sprint), 5)
            for test in sprint_tests(sprint_number)
        ]

    if choice == "4":
        directory = input(
            "Dossier relatif à inout-test (ex. test1 ou comparaison) : "
        ).strip()
        folder = (BASE / directory).resolve()
        if BASE not in folder.parents or not folder.is_dir():
            raise ValueError("Le dossier indiqué est introuvable ou hors du projet.")
        in_file = folder / "in.txt"
        out_file = folder / "out.txt"
        if not in_file.is_file() or not out_file.is_file():
            raise ValueError("Le dossier doit contenir in.txt et out.txt.")
        return [(in_file, out_file)]

    if choice == "5":
        return [(BASE / "inout-sujet" / "in.txt", BASE / "inout-sujet" / "out.txt")]

    raise ValueError("Choix invalide.")


def choose_error_limit() -> int | None:
    value = input(
        "Nombre maximal d'erreurs avant arrêt (0 = illimité) [0] : "
    ).strip() or "0"
    try:
        limit = int(value)
    except ValueError as error:
        raise ValueError("Le nombre d'erreurs doit être un entier positif ou nul.") from error
    if limit < 0:
        raise ValueError("Le nombre d'erreurs doit être positif ou nul.")
    return limit or None


def main():
    try:
        tests = choose_tests()
        error_limit = choose_error_limit()
    except ValueError as error:
        print(f"{RED}{error}{RESET}")
        sys.exit(1)

    if not tests:
        print(f"{RED}Aucun test trouvé.{RESET}")
        sys.exit(1)

    success = True
    errors = 0
    for in_file, out_file in tests:
        test_success, errors, stopped = test_file_pair(
            in_file, out_file, error_limit, errors
        )
        if not test_success:
            success = False
        if stopped:
            print(f"{YELLOW}Arrêt des tests après {errors} erreur(s).{RESET}")
            break
    print(
        f"\n{GREEN}Tous les tests sélectionnés sont réussis.{RESET}"
        if success
        else f"\n{RED}Au moins un test sélectionné a échoué.{RESET}"
    )
    sys.exit(0 if success else 1)


if __name__ == "__main__":
    main()
