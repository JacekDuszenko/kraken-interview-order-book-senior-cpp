import argparse
import csv
import sys
from pathlib import Path
import socket

header = []
parser = argparse.ArgumentParser(
    prog='kraken client',
    description="""parses input csv and sends
    corresponding actions to a order book server."""
)
parser.add_argument("filename")
parser.add_argument("host")
parser.add_argument("port", type=int)


def stream_data_to_server(file: Path, host: str, port: int):
    with open(file) as f:
        reader = csv.reader(f, delimiter=',')
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        for row in reader:
            if len(row) > 0 and "#" not in row[0]:
                data = ",".join(list(map(lambda s: s.strip(), row)))
                print(data)
                sock.sendto(bytes(data, "utf-8"), (host, port))


if __name__ == "__main__":
    args = parser.parse_args()
    target_file = Path(args.filename)
    if not target_file.exists():
        print(f"The target file {target_file} does not exist")
        sys.exit(1)

    print(f"Reading {target_file} and sending to {args.host}:{args.port}")
    stream_data_to_server(target_file, args.host, args.port)
