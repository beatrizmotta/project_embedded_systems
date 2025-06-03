# Projeto de Radar de Velocidade Simplificado - Zephyr RTOS

Projeto desenvolvido usando o Zephyr RTOS para obtenção de nota na disciplina de Sistemas Embarcados na Universidade Federal de Alagoas.

## Alunos:
- Frederico Guilherme de Oliveira Gomes Filho
- Maria Beatriz Motta Barbosa Valente 

## Sobre o projeto:

Esse projeto consiste na implementação de um radar eletrônico de velocidade simplificado, onde, fazendo uso de dois sensores e calculando a diferença do tempo de ativação entre eles, o sistema consegue detectar a velocidade de veículos na via e registrar infrações em um serviço web. 


### Funcionamento:


- **Detecção de Velocidade**: Utiliza dois sensores magnéticos para calcular a velocidade dos veículos
- **Sincronização de Tempo**: Integração com servidor SNTP para timestamp preciso
- **Transmissão de Dados**: Cliente HTTP para envio de dados de infrações para servidor remoto
- **Configuração Flexível**: Sistema totalmente configurável via Kconfig
- **Modo Simulação**: Capacidade de simular o funcionamento para testes e desenvolvimento
- **Validação de Dados**: Sistema de validação de placas veiculares

## Arquitetura do Sistema

### Módulos 

#### 1. Sensor (sensor.c):
Responsável por detectar a passagem dos veículos através dos sensores simulados e calcular sua velocidade com base no tempo. Publica as detecções via ZBUS para que sejam consumidas em outros módulos.

#### 2. Camera Capture (camera_capture.c):
Esse módulo implementa a lógica responsável por reagir à detecção dos sensores, acionando a câmera, validando a placa e colocar na fila da message queue a infração que deve ser enviada para o servidor web.

#### 3. Http Client (http_client.c):
Esse módulo é responsável por enviar as notificações de trânsito ao servidor web via HTTP, utilizando sockets TCP com suporte da network stack do Zephyr. 

#### 4. Http Sender (http_sender.c):
Este módulo define a thread responsável por enviar notificações de infrações de trânsito via HTTP para um servidor remoto. Ele integra a fila de mensagens (k_msgq) com o cliente HTTP de baixo nível implementado em http_client.c.

#### 4. NTP (ntp.c):
Este módulo é responsável por obter a data e hora atual a partir de um servidor NTP (Network Time Protocol), permitindo que o sistema registre eventos com carimbo de tempo preciso, como infrações de trânsito capturadas pela câmera.

### Arquitetura 

O sistema utiliza múltiplas threads:

- **Thread de Sensores**: Monitoramento dos sensores
- **Thread HTTP**: Transmissão dos dados para o servidor HTTP
- **Thread de Captura da Câmera**: Fetching do timestamp atual.
- **Thread da Câmera**: Transmissão das fotos simuladas dos veículos.

## Ambiente de Execução

### Pré-requisitos

- **Zephyr SDK** versão 4.10.0 ou superior
- **Python 3.10+** para o servidor web
- **QEMU** para emulação (opcional)
- **West** (Zephyr's meta-tool)


### Para rodar em sua máquina:

#### Instalação:

```bash
git clone https://github.com/beatrizmotta/project_embedded_systems.git
cd project_embedded_systems

west init -l .
west update

pip install -r requirements.txt
```


#### Execução

```bash
# Para compilar/buildar
west build -b qemu_cortex_m3 -p always . -DDTC_OVERLAY_FILE=qemu.overlay
# Para executar
west build -t run
```

## Configuração

O sistema oferece configurações customizáveis por meio do Kconfig. 
### Principais Opções Configuráveis

#### Sobre detecção da velocidade 

- RADAR_SENSOR_DISTANCE_MM: Distância entre os sensores (em milimetros).
- RADAR_SPEED_LIMIT_KMH: Limite da via em km/h.
- RADAR_CAMERA_FAILURE_RATE_PERCENT: Porcentagem de chance da câmera simular uma falha e retornar uma placa inválida.

#### Sobre o servidor WEB

- SERVER_HOST: Hostname do servidor em nuvem que receberá as infrações.
- SERVER_PORT: Porta do servidor que receberá as infrações.
- SERVER_URL: Rota/endpoint para o qual o servidor enviará a infração.

## Servidor Web 

### Configuração do Servidor Web

O servidor web (`server.py`) implementa uma API REST que vai receber os dados da infração:

```bash
# Executar o servidor
pip install fastapi uvicorn
uvicorn server:app --reload
```

### Endpoints 

- **POST /notify-infraction**: Vai receber os dados da infração e retornar uma mensagem de sucesso ou erro para o usuário (possui validação de campos)

### Exemplo de JSON payload

```json
{
    "plate": "EGG3D02",
    "timestamp": "2025-06-03 16:30:50",
    "image_hash": "0d6b4d62516fe9eeb9ce56c57a6559e21e02436d542af9aef03f322d32d408ed",
    "recorded_speed": 43
}
```
