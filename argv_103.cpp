#include "./argv_102.h"


int main (int argc, char ** argv) {
	if (2 != argc) {
		fprintf (stderr, "Usage: ./PeCertRemover <FileWhoseCertIsToRemove>\n");
	}
	else {
		HANDLE HandleFile = CreateFileA (argv[1], GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ,  NULL, OPEN_EXISTING, 0, NULL);
		if (NULL == HandleFile) {
			fprintf (stderr, "+++ Error: CreateFileA failed (GetLastError=%d)\n", GetLastError());
		}
		else {
			DWORD NbCertificates;
			BOOL Bresult = ImageEnumerateCertificates (HandleFile, CERT_SECTION_TYPE_ANY, &NbCertificates, NULL, 0);
			if (FALSE == Bresult) {
				fprintf (stderr, "+++ Error: ImageEnumerateCertificates failed (GetLastError=%d)\n", GetLastError());
			}
			else if (0 == NbCertificates) {
				fprintf (stderr, "*** Warning: no certificate found for this file (nothing to do)\n");
			}
			else {
				fprintf (stderr, "Found %d certificates for the file\n", NbCertificates);
				for (DWORD i=0 ; (TRUE == Bresult) && (i<NbCertificates) ; i++) {
					Bresult = ImageRemoveCertificate (HandleFile, i);
					if (FALSE == Bresult) {
						fprintf (stderr, "+++ Error: ImageRemoveCertificate failed (while removing certificate %d) - GetLastError=%d\n", i, GetLastError());
					}
					else {
						fprintf (stderr, "removed certificate %d on %d...[OK]\n", i, NbCertificates);
					}
				}
			}
			CloseHandle (HandleFile);
		}
	}
}