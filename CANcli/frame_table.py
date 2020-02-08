from frame_data import FrameData

class FrameTable:
    def __init__(self, name="", max_table_size=16):
        self.name = name
        self.max_table_size = max_table_size
        self.table = {}

    def add(self, frame):
        if(len(self.table) < self.max_table_size or (self.table.get(frame.id) is not None)):
            self.table[frame.id] = frame

    def __str__(self):
        res = "table"
        if(not self.name is None): res += "<" + self.name + ">"
        res += ": (" + str(len(self.table)) + ")"

        for frame in self.table.values(): res += "\n\t" + str(frame)
        return res
