/**
 * FreeRDP: A Remote Desktop Protocol Implementation
 * FreeRDP Mac OS X Server (Audio Input)
 *
 * Copyright 2012 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "mfreerdp.h"

#include "mf_audin.h"

static const rdpsndFormat audio_formats[] =
{
	{ 0x11, 2, 22050, 1024, 4, 0, NULL }, /* IMA ADPCM, 22050 Hz, 2 channels */
	{ 0x11, 1, 22050, 512, 4, 0, NULL }, /* IMA ADPCM, 22050 Hz, 1 channels */
	{ 0x01, 2, 22050, 4, 16, 0, NULL }, /* PCM, 22050 Hz, 2 channels, 16 bits */
	{ 0x01, 1, 22050, 2, 16, 0, NULL }, /* PCM, 22050 Hz, 1 channels, 16 bits */
	{ 0x01, 2, 44100, 4, 16, 0, NULL }, /* PCM, 44100 Hz, 2 channels, 16 bits */
	{ 0x01, 1, 44100, 2, 16, 0, NULL }, /* PCM, 44100 Hz, 1 channels, 16 bits */
	{ 0x01, 2, 11025, 4, 16, 0, NULL }, /* PCM, 11025 Hz, 2 channels, 16 bits */
	{ 0x01, 1, 11025, 2, 16, 0, NULL }, /* PCM, 11025 Hz, 1 channels, 16 bits */
	{ 0x01, 2, 8000, 4, 16, 0, NULL }, /* PCM, 8000 Hz, 2 channels, 16 bits */
	{ 0x01, 1, 8000, 2, 16, 0, NULL } /* PCM, 8000 Hz, 1 channels, 16 bits */
};

static void mf_peer_audin_opening(audin_server_context* context)
{
	printf("AUDIN opening.\n");
	/* Simply choose the first format supported by the client. */
	context->SelectFormat(context, 0);
}

static void mf_peer_audin_open_result(audin_server_context* context, UINT32 result)
{
	printf("AUDIN open result %d.\n", result);
}

static void mf_peer_audin_receive_samples(audin_server_context* context, const void* buf, int nframes)
{
	printf("AUDIN receive %d frames.\n", nframes);
}

void mf_peer_audin_init(mfPeerContext* context)
{
	context->audin = audin_server_context_new(context->vcm);
	context->audin->data = context;

	context->audin->server_formats = audio_formats;
	context->audin->num_server_formats = sizeof(audio_formats) / sizeof(audio_formats[0]);

	context->audin->dst_format.wFormatTag = 1;
	context->audin->dst_format.nChannels = 2;
	context->audin->dst_format.nSamplesPerSec = 44100;
	context->audin->dst_format.wBitsPerSample = 16;

	context->audin->Opening = mf_peer_audin_opening;
	context->audin->OpenResult = mf_peer_audin_open_result;
	context->audin->ReceiveSamples = mf_peer_audin_receive_samples;
}
