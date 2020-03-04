KIPS := sfdnsres_mitm

SUBFOLDERS := Atmosphere-libs/libstratosphere $(KIPS)

TOPTARGETS := all clean

OUTDIR		:=	out
SD_ROOT     :=  $(OUTDIR)/sd
TITLE_DIR   :=  $(SD_ROOT)/atmosphere/contents/0x4200000000000010

$(TOPTARGETS): PACK

$(SUBFOLDERS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(KIPS): Atmosphere-libs/libstratosphere

#---------------------------------------------------------------------------------
PACK: $(SUBFOLDERS)
	@[ -d $(TITLE_DIR)/flags ] || mkdir -p $(TITLE_DIR)/flags
	@ cp sfdnsres_mitm/sfdnsres_mitm.nsp $(TITLE_DIR)/exefs.nsp
	@ touch $(TITLE_DIR)/flags/boot2.flag
#---------------------------------------------------------------------------------

.PHONY: $(TOPTARGETS) $(SUBFOLDERS)