class PipeBuffer {
  constructor() {
    this.sab_ = null;
    this.int32_ = null;
  }
  initializeFromSAB(sab) {
    this.sab_ = sab;
    this.int32_ = new Int32Array(this.sab_);
  }
  initialize(size) {
    this.initializeFromSAB(new SharedArrayBuffer(PipeBuffer.kHeaderSize + size));
  }
  get sab() {
    return this.sab_;
  }
  get int32() {
    return this.int32_;
  }
  get writeCounter() { 
    return Atomics.load(this.int32_, 0);
  }
  incrementWriteCounter() {
    Atomics.add(this.int32_, 0, 1);
  }
  get readCounter() { 
    return Atomics.load(this.int32_, 1);
  }
  incrementReadCounter() {
    Atomics.add(this.int32_, 1, 1);
  }
  get numBytes() { 
    return Atomics.load(this.int32_, 2);
  }
  set numBytes(value) {
    Atomics.store(this.int32_, 2, value);
  }
  get maxBytes() {
    return this.sab_.byteLength - PipeBuffer.kHeaderSize;
  }
  copyBytesOut() {
    var result = new Int8Array(this.numBytes);
    result.set(new Int8Array(this.sab_, PipeBuffer.kHeaderSize, this.numBytes));
    return result;
  }
  copyBytesIn(bytes) {
    var int8 = new Int8Array(this.sab_, PipeBuffer.kHeaderSize, bytes.byteLength);
    int8.set(bytes);
    this.numBytes = bytes.byteLength;
  }
}
PipeBuffer.kHeaderSize =
    4 +  // write counter
    4 +  // read counter
    4;   // num bytes

class PipeReader {
  constructor(buffer) {
    this.buffer_ = buffer;
  }
  read() {  // returns Int8Array, blocking until available
    this.waitForNewInput_();
    var result = this.buffer_.copyBytesOut();
    this.buffer_.incrementReadCounter();
    return result;
  }
  tryRead() {  // returns Int8Array or null
    if (this.buffer_.writeCounter == this.buffer_.readCounter)
      return null;
    var result = this.buffer_.copyBytesOut();
    this.buffer_.incrementReadCounter();
    return result;
  }
  waitForNewInput_() {
    while (this.buffer_.writeCounter == this.buffer_.readCounter)
      Atomics.wait(this.buffer_.int32, 0, this.buffer_.readCounter);
  }
}

class PipeWriter {
  constructor(buffer) {
    this.buffer_ = buffer;
    this.send_queue_ = new Array();
  }
  write(bytes) {
    var maxBytes = this.buffer_.maxBytes;
    if (bytes.byteLength > maxBytes) {
      throw "oops: message is too long to send!";
    } else {
      this.send_queue_.push(bytes);
    }
    this.doPendingWrites();
  }
  hasPendingWrites() {
    return this.send_queue_.length > 0;
  }
  doPendingWrites() {
    while (this.hasPendingWrites()) {
      if (!this.trySendNext_())
        break;
    }
  }
  trySendNext_() {
    if (this.buffer_.writeCounter != this.buffer_.readCounter)
      return false;  // Waiting for reader
    var bytes = this.send_queue_.pop();
    this.buffer_.copyBytesIn(bytes);
    this.buffer_.incrementWriteCounter();
    Atomics.wake(this.buffer_.int32, 0, 1);
    return true;
  }
}
