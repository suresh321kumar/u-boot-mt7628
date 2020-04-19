int tftp_config(int type, char *argv[])
{
	char *s;
	char default_file[ARGV_LEN], file[ARGV_LEN], devip[ARGV_LEN], srvip[ARGV_LEN], default_ip[ARGV_LEN];

	//printf(" Please Input new ones /or Ctrl-C to discard\n");

	memset(default_file, 0, ARGV_LEN);
	memset(file, 0, ARGV_LEN);
	memset(devip, 0, ARGV_LEN);
	memset(srvip, 0, ARGV_LEN);
	memset(default_ip, 0, ARGV_LEN);

	printf("\tInput device IP ");
	s = "192.168.7.1";
	memcpy(devip, s, strlen(s));
	memcpy(default_ip, s, strlen(s));

	printf("(%s) ", devip);
	input_value(devip);
	setenv("ipaddr", devip);
	if (strcmp(default_ip, devip) != 0)
		modifies++;

	printf("\tInput server IP ");
	s = "192.168.7.2";
	memcpy(srvip, s, strlen(s));
	memset(default_ip, 0, ARGV_LEN);
	memcpy(default_ip, s, strlen(s));

	printf("(%s) ", srvip);
	input_value(srvip);
	setenv("serverip", srvip);
	if (strcmp(default_ip, srvip) != 0)
		modifies++;

	if(type == SEL_LOAD_BOOT_SDRAM 
			|| type == SEL_LOAD_BOOT_WRITE_FLASH 
#ifdef RALINK_UPGRADE_BY_SERIAL
			|| type == SEL_LOAD_BOOT_WRITE_FLASH_BY_SERIAL
#endif
			) {
		if(type == SEL_LOAD_BOOT_SDRAM)
#if defined (RT2880_ASIC_BOARD) || defined (RT2880_FPGA_BOARD)
			argv[1] = "0x8a200000";
#else
		argv[1] = "0x80200000";
#endif
		else
#if defined (RT2880_ASIC_BOARD) || defined (RT2880_FPGA_BOARD)
			argv[1] = "0x8a100000";
#else
		argv[1] = "0x80100000";
#endif
		printf("\tInput Uboot filename ");
		//argv[2] = "uboot.bin";
		strncpy(argv[2], "uboot.bin", ARGV_LEN);
	}
	else if (type == SEL_LOAD_LINUX_WRITE_FLASH) {
#if defined (RT2880_ASIC_BOARD) || defined (RT2880_FPGA_BOARD)
		argv[1] = "0x8a100000";
#else
		argv[1] = "0x80100000";
#endif
		printf("\tInput Linux Kernel filename ");
		//argv[2] = "uImage"; winfred: use strncpy instead to prevent the buffer overflow at copy_filename later
		strncpy(argv[2], "uImage", ARGV_LEN);
	}
	else if (type == SEL_LOAD_LINUX_SDRAM ) {
		/* bruce to support ramdisk */
#if defined (RT2880_ASIC_BOARD) || defined (RT2880_FPGA_BOARD)
		argv[1] = "0x8a800000";
#else
		argv[1] = "0x80A00000";
#endif
		printf("\tInput Linux Kernel filename ");
		//argv[2] = "uImage";
		strncpy(argv[2], "uImage", ARGV_LEN);
	}

	s = "firm_initramfs.bin";
	if (s != NULL) {
		memcpy(file, s, strlen(s));
		memcpy(default_file, s, strlen(s));
	}
	printf("(%s) ", file);
	input_value(file);
	if (file == NULL)
		return 1;
	copy_filename (argv[2], file, sizeof(file));
	setenv("bootfile", file);
	if (strcmp(default_file, file) != 0)
		modifies++;

	return 0;
}