from pymodbus.server.async_io import ModbusTcpServer
from pymodbus.server.async_io import ModbusSerialServer
from pymodbus.datastore import ModbusSequentialDataBlock
from pymodbus.datastore import ModbusSlaveContext, ModbusServerContext
from pymodbus.transaction import ModbusRtuFramer

import time
import sys
import threading
from qtpy impot QtWidgets


# Initialize the server context and data blocks
block = ModbusSequentialDataBlock(0, [0] * 100)
store = ModbusSlaveContext(di=None, co=None, hr=block, ir=None)
context = ModbusServerContext(slaves=store, single=True)

# Create a serial Modbus server on COM5 with baud rate of 115200
server = ModbusSerialServer(context, framer=ModbusRtuFramer, port='COM5', baudrate=115200)

# Start the server
server.start()

