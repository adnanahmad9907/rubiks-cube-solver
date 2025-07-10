import kociemba

cube_state = (
    "UUUUUUUUU"  # U face
    "RRRRRRRRR"  # R face
    "FFFFFFFFF"  # F face
    "DDDDDDDDD"  # D face
    "LLLLLLLLL"  # L face
    "BBBBBBBBB"  # B face
)

solution = kociemba.solve(cube_state)
print("Solution:", solution)
