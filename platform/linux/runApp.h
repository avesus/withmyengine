# include <GLES2/gl2.h>

# include <cstdio>

void runApp () {

	printf("GL_VERSION  : %s\n", glGetString(GL_VERSION) );
	printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER) );

	glViewport(0, 0, 64, 64);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

/* Create swap chain GPGPU render textures
const gpgpuTexture = [,];
for (let i = 0; i < 2; ++i) {
  gpgpuTexture[i] = {
    texture: gl.createTexture(),
    framebuffer: gl.createFramebuffer()
  };
  gl.bindTexture(gl.TEXTURE_2D, gpgpuTexture[i].texture);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
  gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gpgpuTextureSide, gpgpuTextureSide, 0, gl.RGBA, gl.UNSIGNED_BYTE, data);
  gl.bindFramebuffer(gl.FRAMEBUFFER, gpgpuTexture[i].framebuffer);
  gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, gpgpuTexture[i].texture, 0);
}
*/

	char* data[64*64*8] = { 0 };

	//glBindFramebuffer(GL_FRAMEBUFFER, 1);//gpgpuTexture[chainIdSrc].framebuffer);
	glReadPixels(0, 0, 64, 64, GL_RGBA, GL_UNSIGNED_BYTE, data);

}

