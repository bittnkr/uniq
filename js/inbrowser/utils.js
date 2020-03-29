function checkSharedArrayBufferEnabled() {
  try { new SharedArrayBuffer() }
  catch (e) {
    add.p('Your Browser have SharedArrayBuffer Disabled. <br>To run these tests you need to enable this feature. <br>Copy and paste the link below in the address bar and Enable the option ')
    add.p('<strong>chrome://flags/#enable-webassembly-threads<br>')
  }
}
