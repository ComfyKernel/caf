<h1 align="center">CAF</h1>
<h3 align="center"><em>Comfy Asset File</em></h3>

<h4><em>What is it?</em></h4>
CAF is a way of storing asset files independant of the systems filesystem.<br />

<h4><em>Why did you make it?</em></h4>
I was getting pretty bored with the normal way of loading files, why open 15 streams for images when I can just open one?<br />
It also support some neat features due to how basic it is, 'symlinks' are possible because at build time the lumpitem can direct it's pointer to another asset<br />

<h3>Specs (for V1.1)</h3>
<h4>[HEAD]</h4>
<table>
  <thead>
    <th>Description</th>
    <th>Type</th>
    <th>Size</th>
  </thead>
  <tbody>
    <tr>
      <td>"CAF" In ASCII Text [ 0x43, 0x41, 0x46 ]</td>
      <td align="center">char</td>
      <td align="center">3-bytes</td>
    </tr>
    <tr>
      <td><strong>CAF</strong> Version [ Major ]</td>
      <td align="center">uint16_t</td>
      <td align="center">2-bytes</td>
    </tr>
    <tr>
      <td><strong>CAF</strong> Version [ Minor ]</td>
      <td align="center">uint16_t</td>
      <td align="center">2-bytes</td>
    </tr>
    <tr>
      <td>Asset File Revision</td>
      <td align="center">uint32_t</td>
      <td align="center">4-bytes</td>
    </tr>
    <tr>
      <td>Pointer to first <strong>LUMPITEM</strong></td>
      <td align="center">uint64_t</td>
      <td align="center">8-bytes</td>
    </tr>
    <tr>
      <td><strong>Asset Root</strong> Name [ NULL-Terminated String ]</td>
      <td align="center">char</td>
      <td align="center">null term</td>
    </tr>
  </tbody>
</table>

<h4>[LUMP ITEM]</h4>
<h6>Note: Each flag is 1 Byte long, making both flags take up 2 bytes of space</h6>
<table>
  <thead>
    <th>Flag 0</th>
    <th>Description</th>
    <th>Flag 1</th>
    <th>Description</th>
  </thead>
  <tbody>
    <tr>
      <td>B0</td>
      <td>[ITEM] Next Item Exists</td>
      <td>B0</td>
      <td>Reserved for file type</td>
    </tr>
    <tr>
      <td>B1</td>
      <td>[IGNORE] Tell reader to ignore this lump</td>
      <td>B1</td>
      <td>Reserved for file type</td>
    </tr>
    <tr>
      <td>B2</td>
      <td>[EMPTY] Tell reader to not load lump data</td>
      <td>B2</td>
      <td>Reserved for file type</td>
    </tr>
    <tr>
      <td>B3</td>
      <td>[FILE] Tell reader to load file from the system</td>
      <td>B3</td>
      <td>Reserved for file type</td>
    </tr>
    <tr>
      <td>B4</td>
      <td>Unused</td>
      <td>B4</td>
      <td>Reserved for file type</td>
    </tr>
    <tr>
      <td>B5</td>
      <td>Unused</td>
      <td>B5</td>
      <td>Reserved for file type</td>
    </tr>
    <tr>
      <td>B6</td>
      <td>Unused</td>
      <td>B6</td>
      <td>Reserved for file type</td>
    </tr>
    <tr>
      <td>B7</td>
      <td>Unused</td>
      <td>B7</td>
      <td>Reserved for file type</td>
    </tr>
  </tbody>
</table>

<table>
  <thead>
    <th>Description</th>
    <th>Type</th>
    <th>Size</th>
  </thead>
  <tbody>
    <tr>
      <td>Lump Size</td>
      <td align="center">uint64_t</td>
      <td align="center">8-bytes</td>
    </tr>
    <tr>
      <td>Lump Revision</td>
      <td align="center">uint32_t</td>
      <td align="center">4-bytes</td>
    </tr>
    <tr>
      <td>Pointer to <strong>LUMP</strong></td>
      <td align="center">uint64_t</td>
      <td align="center">8-bytes</td>
    </tr>
    <tr>
      <td>Pointer to next <strong>LUMP ITEM</strong></td>
      <td align="center">uint64_t</td>
      <td align="center">8-bytes</td>
    </tr>
    <tr>
      <td>Name [ NULL-Terminated String ]</td>
      <td align="center">char</td>
      <td align="center">null-term</td>
    </tr>
    <tr>
      <td>Path [ NULL-Terminated String ]</td>
      <td align="center">char</td>
      <td align="center">null-term</td>
    </tr>
    <tr>
      <td>Type [ NULL-Terminated String ]</td>
      <td align="center">char</td>
      <td align="center">null-term</td>
    </tr>
  </tbody>
</table>

<h4>[LUMP]</h4>
It can be anywhere in the file and of any size fitting into a 'super' (uint64_t) integer. This allows lumps to be either shoved infront of the items, below the items, or in ComfyTool's CAF Builders case, between each item.
