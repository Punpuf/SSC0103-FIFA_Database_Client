import pathlib
import socket
import json
import subprocess
import os
from typing import List

HOST = "127.0.0.1"
PORT = 5000

FILES_DIR = "db_out"
NOME_ARQUIVO_INDICE = f"{FILES_DIR}/indice.bin"
NOME_ARQUIVO_DADOS = f"{FILES_DIR}/dados.bin"
NOME_TEMP_CSV = f"{FILES_DIR}/temp.csv"


class Registro:
    id: int
    idade: int | None
    nome_jogador: str | None
    nacionalidade: str | None
    nome_clube: str | None

    def __init__(
        self,
        *,
        id: int,
        idade: int | None = None,
        nome_jogador: str | None = None,
        nacionalidade: str | None = None,
        nome_clube: str | None = None,
    ) -> None:
        self.id = id
        self.idade = idade
        self.nome_jogador = nome_jogador
        self.nacionalidade = nacionalidade
        self.nome_clube = nome_clube

    @classmethod
    def from_dict(cls, d: dict):
        return cls(
            id=d["id"],
            idade=d.get("idade", None),
            nome_jogador=d.get("nomeJogador", None),
            nacionalidade=d.get("nacionalidade", None),
            nome_clube=d.get("nomeClube", None),
        )

    def to_dict(self):
        return {
            "id": self.id,
            "idade": self.idade,
            "nomeJogador": self.nome_jogador,
            "nacionalidade": self.nacionalidade,
            "nomeClube": self.nome_clube,
        }

    def para_seletor(self):
        return Seletor(
            id=self.id,
            idade=self.idade,
            nome_jogador=self.nome_jogador,
            nacionalidade=self.nacionalidade,
            nome_clube=self.nome_clube,
        )

    def __repr__(self) -> str:
        return f"Registro(id={self.id}, idade={self.idade}, nome_jogador={self.nome_jogador}, nacionalidade={self.nacionalidade}, nome_clube={self.nome_clube})"


class Seletor:
    id: int | None
    idade: int | None
    nome_jogador: str | None
    nacionalidade: str | None
    nome_clube: str | None

    def __init__(
        self,
        *,
        id: int | None = None,
        idade: int | None = None,
        nome_jogador: str | None = None,
        nacionalidade: str | None = None,
        nome_clube: str | None = None,
    ) -> None:
        self.id = id
        self.idade = idade
        self.nome_jogador = nome_jogador
        self.nacionalidade = nacionalidade
        self.nome_clube = nome_clube

    @classmethod
    def from_dict(cls, d: dict):
        return cls(
            id=d.get("id", None),
            idade=d.get("idade", None),
            nome_jogador=d.get("nomeJogador", None),
            nacionalidade=d.get("nacionalidade", None),
            nome_clube=d.get("nomeClube", None),
        )

    def to_dict(self):
        return {
            "id": self.id,
            "idade": self.idade,
            "nomeJogador": self.nome_jogador,
            "nacionalidade": self.nacionalidade,
            "nomeClube": self.nome_clube,
        }


def executar_programa(entrada: str):
    return subprocess.run(
        ["./c_src/programaTrab"],
        text=True,
        input=entrada,
        capture_output=True,
    )


class Entradas:
    @staticmethod
    def criar_arquivo_dados(nome_csv: str):
        entrada = f"1 {nome_csv} {NOME_ARQUIVO_DADOS}\n"
        return entrada

    @staticmethod
    def buscar_registros(seletor: Seletor):
        entrada = f"3 {NOME_ARQUIVO_DADOS} {1}\n"

        d = seletor.to_dict()
        campos = list((k, v) for k, v in d.items() if v is not None)

        linha = " ".join(
            map(
                lambda par: f"{par[0]} {par[1]}"
                if isinstance(par[1], int)
                else f'{par[0]} "{par[1]}"',
                campos,
            )
        )

        entrada += f"{len(campos)} {linha}\n"

        return entrada

    @staticmethod
    def inserir_registros(registros: List[Registro]):
        entrada = f"6 {NOME_ARQUIVO_DADOS} {NOME_ARQUIVO_INDICE} {len(registros)}\n"

        def campo(valor: str | int | None):
            if valor is None:
                return "NULO"
            if isinstance(valor, int):
                return valor
            return f'"{valor}"'

        for registro in registros:
            id = registro.id
            idade = campo(registro.idade)
            nome_jogador = campo(registro.nome_jogador)
            nacionalidade = campo(registro.nacionalidade)
            nome_clube = campo(registro.nome_clube)

            entrada += f"{id} {idade} {nome_jogador} {nacionalidade} {nome_clube}\n"

        return entrada

    @staticmethod
    def remover_registros(seletores: List[Seletor]):
        entrada = f"5 {NOME_ARQUIVO_DADOS} {NOME_ARQUIVO_INDICE} {len(seletores)}\n"

        for seletor in seletores:
            quantidade = 0
            filtro = ""
            if seletor.id is not None:
                filtro += f"id {seletor.id} "
                quantidade += 1
            if seletor.idade is not None:
                filtro += f"idade {seletor.idade} "
                quantidade += 1
            if seletor.nome_jogador is not None:
                filtro += f"nomeJogador {seletor.nome_jogador} "
                quantidade += 1
            if seletor.nacionalidade is not None:
                filtro += f"nacionalidade {seletor.nacionalidade} "
                quantidade += 1
            if seletor.nome_clube is not None:
                filtro += f"nomeClube {seletor.nome_clube} "
                quantidade += 1
            entrada += f"{quantidade} {filtro}\n"

        return entrada


class Saidas:
    @staticmethod
    def buscar_registros(saida: str):
        registros: List[Registro] = []

        def transformar(campo: str, classe: type):
            if campo == "SEM DADO":
                return None
            return classe(campo)

        id: int | None = None
        idade: int | None = None
        nome_jogador: str | None = None
        nacionalidade: str | None = None
        nome_clube: str | None = None

        for linha in saida.split("\n"):
            linha = linha.strip()

            if not linha or linha == "Busca 1":
                continue
            if linha == "Registro inexistente.":
                return registros

            chave, valor = linha.split(":", maxsplit=1)

            valor = valor.strip()
            if chave == "ID":
                id = transformar(valor, int)
            elif chave == "Idade":
                idade = transformar(valor, int)
            elif chave == "Nome do Jogador":
                nome_jogador = transformar(valor, str)
            elif chave == "Nacionalidade do Jogador":
                nacionalidade = transformar(valor, str)
            elif chave == "Clube do Jogador":
                nome_clube = transformar(valor, str)
                # esse campo é sempre o último a ser exibido por registro
                registros.append(
                    Registro(
                        id=id,
                        idade=idade,
                        nome_jogador=nome_jogador,
                        nacionalidade=nacionalidade,
                        nome_clube=nome_clube,
                    )
                )
                id = None
                idade = None
                nome_jogador = None
                nacionalidade = None
                nome_clube = None

        if id is not None:
            registros.append(
                Registro(
                    id=id,
                    idade=idade,
                    nome_jogador=nome_jogador,
                    nacionalidade=nacionalidade,
                    nome_clube=nome_clube,
                )
            )

        return registros


def arquivo_dados_existe():
    return os.path.isfile(NOME_ARQUIVO_DADOS)


# FIXME: se o cliente mandar duas mensagems
# em um curto intervalo de tempo, o parser de json
# vai acabar lendo as duas de uma vez, ao invés de
# ler cada uma separadamente, fazendo com que ambas as requisições
# falhem
class RequestHandler:
    s: socket.socket

    def __init__(self, socket: socket.socket):
        self.s = socket

    def handle(self, message: str):
        print("\nReceived request!")
        data = json.loads(message)
        print(f"Request data: {data}")
        tipo = str(data["tipo"])

        if tipo != "carregar" and not arquivo_dados_existe():
            self.enviar_resposta(
                {
                    "tipo": "resultado",
                    "operacao": tipo,
                    "status": "erro",
                    "mensagem": "Arquivo de dados não existe",
                }
            )
            return

        if tipo == "carregar":
            self.carregar_csv(data["caminhoCsv"])
            self.enviar_resposta(
                {"tipo": "resultado", "operacao": tipo, "status": "ok"}
            )
        elif tipo == "inserir":
            self.inserir_registro(Registro.from_dict(data["registro"]))
            self.enviar_resposta(
                {"tipo": "resultado", "operacao": tipo, "status": "ok"}
            )
        elif tipo == "remover":
            self.remover_registros(Seletor.from_dict(data["seletor"]))
            self.enviar_resposta(
                {"tipo": "resultado", "operacao": tipo, "status": "ok"}
            )
        elif tipo == "atualizar":
            self.atualizar_registro(Registro.from_dict(data["registro"]))
            self.enviar_resposta(
                {"tipo": "resultado", "operacao": tipo, "status": "ok"}
            )
        elif tipo == "buscar":
            registros = self.buscar_registros(Seletor.from_dict(data["seletor"]))
            self.enviar_resposta(
                {
                    "tipo": "resultado",
                    "operacao": tipo,
                    "status": "ok",
                    "nro_registros": len(registros),
                }
            )

            for registro in registros:
                self.enviar_resposta(registro.to_dict())
        elif tipo == "exportar_csv":
            csv_str = self.exportar_csv()
            self.enviar_resposta(
                {"tipo": "resultado", "operacao": tipo, "status": "ok", "csv": csv_str}
            )

    def inserir_registro(self, registro: Registro):
        entrada = Entradas.inserir_registros([registro])
        executar_programa(entrada)

    def remover_registros(self, seletor: Seletor):
        entrada = Entradas.remover_registros([seletor])
        executar_programa(entrada)

    def carregar_csv(self, caminho_csv: str):
        # o arquivo de dados já vai ser sobrescrito pelo
        # programa, então não precisamos nos preocupar em
        # remover ele
        pathlib.Path.unlink(NOME_ARQUIVO_INDICE, missing_ok=True)
        entrada = Entradas.criar_arquivo_dados(caminho_csv)
        executar_programa(entrada)

    def buscar_registros(self, seletor: Seletor):
        entrada = Entradas.buscar_registros(seletor)
        p = executar_programa(entrada)
        registros = Saidas.buscar_registros(p.stdout)
        return registros

    def atualizar_registro(self, registro: Registro):
        self.remover_registros(Seletor(id=registro.id))
        self.inserir_registro(registro)

    def exportar_csv(self):
        registros = self.buscar_registros(Seletor())

        csv_str = "id,idade,nomeJogador,nacionalidade,nomeClube\n"

        def campo(valor: str | int | None):
            if valor is None:
                return ""
            return valor

        for registro in registros:
            id = registro.id
            idade = campo(registro.idade)
            nome_jogador = campo(registro.nome_jogador)
            nacionalidade = campo(registro.nacionalidade)
            nome_clube = campo(registro.nome_clube)
            csv_str += f"{id},{idade},{nome_jogador},{nacionalidade},{nome_clube}\n"

        return csv_str

    def enviar_resposta(self, dados: dict):
        self.s.sendall(json.dumps(dados).encode())
        self.s.sendall(b"\n")
        print(f"Response sent: {dados}")


if not os.path.exists(FILES_DIR):
    os.makedirs(FILES_DIR)

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((HOST, PORT))
server.listen()

print(f"Server running on {HOST}:{PORT}")

conn, addr = server.accept()
handler = RequestHandler(conn)


current_message = bytearray()
received_messages: List[str] = []

while True:
    try:
        data = conn.recv(1024)

        for byte in data:
            if byte == b"\n"[0] and current_message != b"\n":
                received_messages.append(current_message.decode())
                current_message.clear()
            else:
                current_message.append(byte)

        for message in received_messages:
            handler.handle(message)

        received_messages = []
    except KeyboardInterrupt:
        print("Servidor fechado")
        exit()
